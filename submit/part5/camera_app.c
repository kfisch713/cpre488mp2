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

static uint16_t average_vert(int i, volatile Xuint16 * mem);
static uint16_t average_hor(int i, volatile Xuint16 * mem);
static uint16_t average_x(int i, volatile Xuint16 * mem);
static uint8_t *color_lut;
static void fill_color_lut();
camera_config_t camera_config;

enum color {
	RED,
	GREEN,
	BLUE
};

int HEIGHT;
int WIDTH;
int FRAME_LEN;

// Main function. Initializes the devices and configures VDMA
int main() {

	printf("made it\n");

	camera_config_init(&camera_config);
	fmc_imageon_enable(&camera_config);
	camera_loop(&camera_config);

	return 0;
}


// Initialize the camera configuration data structure
void camera_config_init(camera_config_t *config) {
	printf("Made it camera_config_init\r\n");

    config->uBaseAddr_IIC_FmcIpmi = XPAR_IIC_FMC_BASEADDR;
    config->uBaseAddr_IIC_FmcImageon = XPAR_FMC_IMAGEON_IIC_0_BASEADDR;
    config->uBaseAddr_VITA_Receiver = XPAR_FMC_IMAGEON_VITA_RECEIVER_0_BASEADDR;
//    config->uBaseAddr_CFA = XPAR_CFA_0_BASEADDR;
//    config->uBaseAddr_CRES = XPAR_CRESAMPLE_0_BASEADDR;
//    config->uBaseAddr_RGBYCC = XPAR_RGB2YCRCB_0_BASEADDR;
    config->uBaseAddr_TPG_PatternGenerator = XPAR_AXI_TPG_0_BASEADDR;

    config->uDeviceId_VTC_ipipe = XPAR_V_TC_0_DEVICE_ID;
    config->uDeviceId_VTC_tpg   = XPAR_V_TC_1_DEVICE_ID;

    config->uDeviceId_VDMA_HdmiFrameBuffer = XPAR_AXI_VDMA_0_DEVICE_ID;
    config->uBaseAddr_MEM_HdmiFrameBuffer = XPAR_DDR_MEM_BASEADDR + 0x10000000;
    config->uNumFrames_HdmiFrameBuffer = XPAR_AXIVDMA_0_NUM_FSTORES;

    return;
}

// Main (SW) processing loop. Recommended to have an explicit exit condition
void camera_loop(camera_config_t *config) {
	printf("Made it camera_loop\r\n");
	WIDTH =  config->hdmio_width;
	HEIGHT = config->hdmio_height;
	FRAME_LEN = WIDTH * HEIGHT;
	color_lut = malloc(FRAME_LEN * sizeof(*color_lut));
	if (color_lut == NULL)
		xil_printf("\n\n\n\n\n\nYOU DON F'ED UP!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n\n\n\n");
	fill_color_lut();
	Xuint32 parkptr;
	Xuint32 vdma_S2MM_DMACR, vdma_MM2S_DMACR;
	int i, j;


	xil_printf("Entering main SW processing loop\r\n");


	// Grab the DMA parkptr, and update it to ensure that when parked, the S2MM side is on frame 0, and the MM2S side on frame 1
	parkptr = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_PARKPTR_OFFSET);
	parkptr &= ~XAXIVDMA_PARKPTR_READREF_MASK;
	parkptr &= ~XAXIVDMA_PARKPTR_WRTREF_MASK;
	parkptr |= 0x1;
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_PARKPTR_OFFSET, parkptr);

	// Grab the DMA Control Registers, and clear circular park mode.
	vdma_MM2S_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET);
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_MM2S_DMACR & ~XAXIVDMA_CR_TAIL_EN_MASK);
	vdma_S2MM_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET);
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_S2MM_DMACR & ~XAXIVDMA_CR_TAIL_EN_MASK);

	// Pointers to the S2MM memory frame and M2SS memory frame
	volatile Xuint16 *pS2MM_Mem = (Xuint16 *)XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_S2MM_ADDR_OFFSET+XAXIVDMA_START_ADDR_OFFSET);
	volatile Xuint16 *pMM2S_Mem = (Xuint16 *)XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_MM2S_ADDR_OFFSET+XAXIVDMA_START_ADDR_OFFSET+4);

	uint16_t R, G, B;
	uint16_t Y, CB, CR;

	printf("Made it before loop\r\n");
	printf("Width: %d, Height: %d\n", WIDTH, HEIGHT);

	// Run for 1000 frames before going back to HW mode
	for (j = 0; j < 1000; j++) {
		for (i = 0 ; i < WIDTH*HEIGHT; i++) {
			//pMM2S_Mem[i] = pS2MM_Mem[1920*1080-i-1] % 255; // made it all very green!
			//pMM2S_Mem[i] = pS2MM_Mem[1920*1080-i+j-1]; // makes the image slowly shift to the right and wrap around.
			//pMM2S_Mem[i] = pS2MM_Mem[i];

			switch (color_lut[i]) {
				case RED:
					R = pS2MM_Mem[i] & 0xFF;
					G = average_vert(i, pS2MM_Mem);
					B = average_x(i, pS2MM_Mem);
					break;
				case GREEN:
					G = pS2MM_Mem[i] & 0xFF;
					if ( (i+1 < FRAME_LEN) && (color_lut[i+1] == RED)) {
						R = average_hor(i, pS2MM_Mem);
						B = average_vert(i, pS2MM_Mem);
					} else if ( (i-1 > 0) && (color_lut[i-1] == BLUE)){
						R = average_vert(i, pS2MM_Mem);
						B = average_hor(i, pS2MM_Mem);
					}
					break;
				case BLUE:
					R = average_x(i, pS2MM_Mem);
					G = average_vert(i, pS2MM_Mem);
					B = pS2MM_Mem[i] & 0xFF;
					break;
				default:
					R = 0;
					G = 0;
					B = 0;
					break;
			}
//			R = 82;
//			G = 232;
//			B = 255;

			//printf("R: %d, G: %d, B: %d\r\n", R, G, B);

			Y  = ( 0.183 * R + 0.614 * G + 0.062 * B) + 16;
			CB = (-0.101 * R - 0.338 * G + 0.439 * B) + 128;
			CR = ( 0.439 * R - 0.399 * G - 0.040 * B) + 128;

			//printf("Y: %X, Cb: %X, Cr: %X\r\n", Y, CB, CR);

//			Y = Y & 0xFF00;
//			CB = CB & 0xFF00;
//			CR = CR & 0xFF00;

//			pMM2S_Mem[i] = pS2MM_Mem[i];

			//Cr Y
			if (i%2){
				pMM2S_Mem[i] =  CR<<8 | Y;
			}
			//Cb Y
			else {
				pMM2S_Mem[i] =  CB<<8 | Y;
			}


		}
	}


	// Grab the DMA Control Registers, and re-enable circular park mode.
	vdma_MM2S_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET);
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_MM2S_DMACR | XAXIVDMA_CR_TAIL_EN_MASK);
	vdma_S2MM_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET);
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_S2MM_DMACR | XAXIVDMA_CR_TAIL_EN_MASK);


	xil_printf("Main SW processing loop complete!\r\n");


	return;
}

static uint16_t average_vert(int i, volatile Xuint16 * mem) {
	int average = 0;
	int count = 0;
	// Not on the top bound
	if (i > WIDTH){
		count++;
		average += mem[i - WIDTH] & 0xFF;
	}
	// Not on the bottom bound
	if (i < FRAME_LEN - WIDTH) {
		count++;
		average += mem[i + WIDTH] & 0xFF;
	}

	average /= count;
	return average;
}

static uint16_t average_hor(int i, volatile Xuint16 * mem) {
	int average = 0;
	int count = 0;
	// Not on the left bound
	if (i % WIDTH != 0) {
		count++;
		average += mem[i - 1] & 0xFF;
	}
	//Not on the right bound
	if ((i % WIDTH) != (WIDTH-1)) {
		count++;
		average += mem[i + 1] & 0xFF;
	}

	average /= count;
	return average;
}

static uint16_t average_x(int i, volatile Xuint16 * mem) {
	int average = 0;
	int count = 0;

	// Top Left
	if (i > WIDTH && ((i % WIDTH) != 0)) {
		count++;
		average += mem[i - WIDTH -1] & 0xFF;
	}
	// Top Right
	if (i > WIDTH && ((i % WIDTH) != (WIDTH-1))) {
		count++;
		average += mem[i - WIDTH +1] & 0xFF;
	}
	// Bottom Left
	if (i < FRAME_LEN && ((i % WIDTH) != 0)) {
		count++;
		average += mem[i + WIDTH -1] & 0xFF;
	}
	// Bottom Right
	if (i < FRAME_LEN && ((i % WIDTH) != (WIDTH-1))) {
		count++;
		average += mem[i + WIDTH +1] & 0xFF;
	}

	average /= count;
	return average;
}

static void fill_color_lut() {
	uint32_t y, x;
	for (y = 0; y < HEIGHT; y++) {
		for(x = 0; x < WIDTH; x = x+2) {
			if (y %2) {
				color_lut[y*WIDTH + x] = GREEN;
				color_lut[y*WIDTH + x+1] = BLUE;
			} else {
				color_lut[y*WIDTH + x] = RED;
				color_lut[y*WIDTH + x+1] = GREEN;
			}
		}
	}


}
