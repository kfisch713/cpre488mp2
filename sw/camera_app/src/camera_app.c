/*****************************************************************************
 * Joseph Zambreno
 * Phillip Jones
 *
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/

/*****************************************************************************
 * camera_app.c - main camera application code. The camera configures the various
 * video in and video out peripherals, and (optionally) performs some
 * image processing on data coming in from the vdma.
 *
 *
 * NOTES:
 * 02/04/14 by JAZ::Design created.
 *****************************************************************************/

#include "camera_app.h"
#include <stdint.h>
#include <xparameters.h>

static uint16_t average_vert(int i, volatile Xuint16 * mem);
static uint16_t average_hor(int i, volatile Xuint16 * mem);
static uint16_t average_x(int i, volatile Xuint16 * mem);

/* Added for software pass-through */
static uint8_t *color_lut;
static void fill_color_lut();
static void camera_interface_init();
static void camera_interface_free();
static void camera_interface();
static void clear_circ_park(camera_config_t *);
static void enable_circ_park(camera_config_t *);
static void display_raw_image(unsigned int index);
camera_config_t camera_config;

static int HEIGHT;
static int WIDTH;
static int FRAME_LEN;

/* Added for camera_interfaceing */
#define MAX_RAW_IMAGES 32
uint16_t *raw_images[MAX_RAW_IMAGES];
static int NUM_SAVED_IMAGES = -1;
static unsigned int curr_image_index = 0;

static Xuint32 vdma_S2MM_DMACR, vdma_MM2S_DMACR;
static Xuint32 parkptr;

enum camera_mode{
    MODE_PASS_THROUGH,
    MODE_PLAY_BACK
};

enum button_val {
	BTN_U,
	BTN_R,
	BTN_D,
	BTN_C
};

char *SWs = (char *)XPAR_SWS_8BITS_BASEADDR;
char *BTNs = (char *)XPAR_BTNS_5BITS_BASEADDR;

int SW(unsigned int x) {
	return ((*SWs >> x) & 0x01);
}

int BTN(unsigned int x) {
	return ((*BTNs >> x) & 0x01);
}

enum color {
    RED,
    GREEN,
    BLUE
};

// Main function. Initializes the devices and configures VDMA
int main() {

	printf("made it\n");

	camera_config_init(&camera_config);
	fmc_imageon_enable(&camera_config);
	camera_interface_init();
	camera_interface(&camera_config);
	camera_interface_free();
	return 0;
}

// Initialize the camera configuration data structure
void camera_config_init(camera_config_t *config) {
	printf("Made it camera_config_init\r\n");

    config->uBaseAddr_IIC_FmcIpmi = XPAR_IIC_FMC_BASEADDR;
    config->uBaseAddr_IIC_FmcImageon = XPAR_FMC_IMAGEON_IIC_0_BASEADDR;
    config->uBaseAddr_VITA_Receiver = XPAR_FMC_IMAGEON_VITA_RECEIVER_0_BASEADDR;
    config->uBaseAddr_CFA = XPAR_CFA_0_BASEADDR;
    config->uBaseAddr_CRES = XPAR_CRESAMPLE_0_BASEADDR;
    config->uBaseAddr_RGBYCC = XPAR_RGB2YCRCB_0_BASEADDR;
    config->uBaseAddr_TPG_PatternGenerator = XPAR_AXI_TPG_0_BASEADDR;

    config->uDeviceId_VTC_ipipe = XPAR_V_TC_0_DEVICE_ID;
    config->uDeviceId_VTC_tpg   = XPAR_V_TC_1_DEVICE_ID;

    config->uDeviceId_VDMA_HdmiFrameBuffer = XPAR_AXI_VDMA_0_DEVICE_ID;
    config->uBaseAddr_MEM_HdmiFrameBuffer = XPAR_DDR_MEM_BASEADDR + 0x10000000;
    config->uNumFrames_HdmiFrameBuffer = XPAR_AXIVDMA_0_NUM_FSTORES;

    return;
}

static void camera_interface_init() {
	size_t i;
    /* Zero all of the raw image pixel arrays */
    for (i = 0; i < MAX_RAW_IMAGES; ++i) {
        raw_images[i] = malloc(sizeof(uint16_t) * WIDTH * HEIGHT);
    }
}

static void camera_interface_free() {
	size_t i;
	/* Zero all of the raw image pixel arrays */
	for (i = 0; i < MAX_RAW_IMAGES; ++i) {
		free(raw_images[i]);
	}
}

static void camera_interface(camera_config_t *config) {
	WIDTH =  config->hdmio_width;
	HEIGHT = config->hdmio_height;
	FRAME_LEN = WIDTH * HEIGHT;

	// Grab the DMA parkptr, and update it to ensure that when parked, the S2MM side is on frame 0, and the MM2S side on frame 1
	parkptr = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_PARKPTR_OFFSET);
	parkptr &= ~XAXIVDMA_PARKPTR_READREF_MASK;
	parkptr &= ~XAXIVDMA_PARKPTR_WRTREF_MASK;
	parkptr |= 0x1;
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_PARKPTR_OFFSET, parkptr);

	clear_circ_park(config);
	enable_circ_park(config);

	// Pointers to the S2MM memory frame and M2SS memory frame
	volatile Xuint16 *pS2MM_Mem = (Xuint16 *)XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_S2MM_ADDR_OFFSET+XAXIVDMA_START_ADDR_OFFSET);
	volatile Xuint16 *pMM2S_Mem = (Xuint16 *)XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_MM2S_ADDR_OFFSET+XAXIVDMA_START_ADDR_OFFSET+4);

	int i, j, curr_mode;

	while(!SW(1)) {
		curr_mode = SW(0);
		if (curr_mode == MODE_PASS_THROUGH) { // HARDWARE PASS-THROUGH mode
			// Do nothing for right now.
			if (BTN(BTN_C)) {
				if (NUM_SAVED_IMAGES < MAX_RAW_IMAGES) {
					clear_circ_park(config);
					uint16_t * raw_image = raw_images[curr_image_index];

					for (i = 0; i < FRAME_LEN; ++i) {
						raw_image[i] = pS2MM_Mem[i];
						pMM2S_Mem[i] = pS2MM_Mem[i];
					}
					usleep(30000);
				}
				enable_circ_park(config);
			}
		} else { // PLAY BACK mode
			clear_circ_park(config);
			while(curr_mode == MODE_PLAY_BACK) {
				// update curr_mode
				curr_mode = *SWs;


			}
			enable_circ_park(config);
		}
	}
	return;
}

static void clear_circ_park(camera_config_t *config) {
	// Grab the DMA Control Registers, and clear circular park mode.
	vdma_MM2S_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET);
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_MM2S_DMACR & ~XAXIVDMA_CR_TAIL_EN_MASK);
	vdma_S2MM_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET);
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_S2MM_DMACR & ~XAXIVDMA_CR_TAIL_EN_MASK);

}

static void enable_circ_park(camera_config_t * config) {
	// Grab the DMA Control Registers, and re-enable circular park mode.
	vdma_MM2S_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET);
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_MM2S_DMACR | XAXIVDMA_CR_TAIL_EN_MASK);
	vdma_S2MM_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET);
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_S2MM_DMACR | XAXIVDMA_CR_TAIL_EN_MASK);
}
