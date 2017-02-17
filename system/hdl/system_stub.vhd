-------------------------------------------------------------------------------
-- system_stub.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity system_stub is
  port (
    processing_system7_0_MIO : inout std_logic_vector(53 downto 0);
    processing_system7_0_PS_SRSTB : in std_logic;
    processing_system7_0_PS_CLK : in std_logic;
    processing_system7_0_PS_PORB : in std_logic;
    processing_system7_0_DDR_Clk : inout std_logic;
    processing_system7_0_DDR_Clk_n : inout std_logic;
    processing_system7_0_DDR_CKE : inout std_logic;
    processing_system7_0_DDR_CS_n : inout std_logic;
    processing_system7_0_DDR_RAS_n : inout std_logic;
    processing_system7_0_DDR_CAS_n : inout std_logic;
    processing_system7_0_DDR_WEB_pin : out std_logic;
    processing_system7_0_DDR_BankAddr : inout std_logic_vector(2 downto 0);
    processing_system7_0_DDR_Addr : inout std_logic_vector(14 downto 0);
    processing_system7_0_DDR_ODT : inout std_logic;
    processing_system7_0_DDR_DRSTB : inout std_logic;
    processing_system7_0_DDR_DQ : inout std_logic_vector(31 downto 0);
    processing_system7_0_DDR_DM : inout std_logic_vector(3 downto 0);
    processing_system7_0_DDR_DQS : inout std_logic_vector(3 downto 0);
    processing_system7_0_DDR_DQS_n : inout std_logic_vector(3 downto 0);
    processing_system7_0_DDR_VRN : inout std_logic;
    processing_system7_0_DDR_VRP : inout std_logic;
    IIC_FMC_SCL : inout std_logic;
    IIC_FMC_SDA : inout std_logic;
    fmc_imageon_hdmi_in_0_io_hdmii_spdif_pin : in std_logic;
    fmc_imageon_hdmi_out_0_io_hdmio_spdif_pin : out std_logic;
    fmc_imageon_hdmi_out_0_io_hdmio_video_pin : out std_logic_vector(15 downto 0);
    fmc_imageon_hdmi_out_0_io_hdmio_clk_pin : out std_logic;
    fmc_imageon_video_clk1 : in std_logic;
    fmc_imageon_iic_0_Rst_pin : out std_logic;
    fmc_imageon_iic_0_Sda_pin : inout std_logic;
    fmc_imageon_iic_0_Scl_pin : inout std_logic;
    BTNs_5Bits_GPIO : inout std_logic_vector(4 downto 0);
    SWs_8Bits_GPIO : inout std_logic_vector(7 downto 0);
    fmc_imageon_vita_receiver_0_io_vita_clk_pll_pin : out std_logic;
    fmc_imageon_vita_receiver_0_io_vita_reset_n_pin : out std_logic;
    fmc_imageon_vita_receiver_0_io_vita_trigger_pin : out std_logic_vector(2 downto 0);
    fmc_imageon_vita_receiver_0_io_vita_monitor_pin : in std_logic_vector(1 downto 0);
    fmc_imageon_vita_receiver_0_io_vita_spi_sclk_pin : out std_logic;
    fmc_imageon_vita_receiver_0_io_vita_spi_ssel_n_pin : out std_logic;
    fmc_imageon_vita_receiver_0_io_vita_spi_mosi_pin : out std_logic;
    fmc_imageon_vita_receiver_0_io_vita_spi_miso_pin : in std_logic;
    fmc_imageon_vita_receiver_0_io_vita_clk_out_p_pin : in std_logic;
    fmc_imageon_vita_receiver_0_io_vita_clk_out_n_pin : in std_logic;
    fmc_imageon_vita_receiver_0_io_vita_sync_p_pin : in std_logic;
    fmc_imageon_vita_receiver_0_io_vita_sync_n_pin : in std_logic;
    fmc_imageon_vita_receiver_0_io_vita_data_p_pin : in std_logic_vector(7 downto 0);
    fmc_imageon_vita_receiver_0_io_vita_data_n_pin : in std_logic_vector(7 downto 0)
  );
end system_stub;

architecture STRUCTURE of system_stub is

  component system is
    port (
      processing_system7_0_MIO : inout std_logic_vector(53 downto 0);
      processing_system7_0_PS_SRSTB : in std_logic;
      processing_system7_0_PS_CLK : in std_logic;
      processing_system7_0_PS_PORB : in std_logic;
      processing_system7_0_DDR_Clk : inout std_logic;
      processing_system7_0_DDR_Clk_n : inout std_logic;
      processing_system7_0_DDR_CKE : inout std_logic;
      processing_system7_0_DDR_CS_n : inout std_logic;
      processing_system7_0_DDR_RAS_n : inout std_logic;
      processing_system7_0_DDR_CAS_n : inout std_logic;
      processing_system7_0_DDR_WEB_pin : out std_logic;
      processing_system7_0_DDR_BankAddr : inout std_logic_vector(2 downto 0);
      processing_system7_0_DDR_Addr : inout std_logic_vector(14 downto 0);
      processing_system7_0_DDR_ODT : inout std_logic;
      processing_system7_0_DDR_DRSTB : inout std_logic;
      processing_system7_0_DDR_DQ : inout std_logic_vector(31 downto 0);
      processing_system7_0_DDR_DM : inout std_logic_vector(3 downto 0);
      processing_system7_0_DDR_DQS : inout std_logic_vector(3 downto 0);
      processing_system7_0_DDR_DQS_n : inout std_logic_vector(3 downto 0);
      processing_system7_0_DDR_VRN : inout std_logic;
      processing_system7_0_DDR_VRP : inout std_logic;
      IIC_FMC_SCL : inout std_logic;
      IIC_FMC_SDA : inout std_logic;
      fmc_imageon_hdmi_in_0_io_hdmii_spdif_pin : in std_logic;
      fmc_imageon_hdmi_out_0_io_hdmio_spdif_pin : out std_logic;
      fmc_imageon_hdmi_out_0_io_hdmio_video_pin : out std_logic_vector(15 downto 0);
      fmc_imageon_hdmi_out_0_io_hdmio_clk_pin : out std_logic;
      fmc_imageon_video_clk1 : in std_logic;
      fmc_imageon_iic_0_Rst_pin : out std_logic;
      fmc_imageon_iic_0_Sda_pin : inout std_logic;
      fmc_imageon_iic_0_Scl_pin : inout std_logic;
      BTNs_5Bits_GPIO : inout std_logic_vector(4 downto 0);
      SWs_8Bits_GPIO : inout std_logic_vector(7 downto 0);
      fmc_imageon_vita_receiver_0_io_vita_clk_pll_pin : out std_logic;
      fmc_imageon_vita_receiver_0_io_vita_reset_n_pin : out std_logic;
      fmc_imageon_vita_receiver_0_io_vita_trigger_pin : out std_logic_vector(2 downto 0);
      fmc_imageon_vita_receiver_0_io_vita_monitor_pin : in std_logic_vector(1 downto 0);
      fmc_imageon_vita_receiver_0_io_vita_spi_sclk_pin : out std_logic;
      fmc_imageon_vita_receiver_0_io_vita_spi_ssel_n_pin : out std_logic;
      fmc_imageon_vita_receiver_0_io_vita_spi_mosi_pin : out std_logic;
      fmc_imageon_vita_receiver_0_io_vita_spi_miso_pin : in std_logic;
      fmc_imageon_vita_receiver_0_io_vita_clk_out_p_pin : in std_logic;
      fmc_imageon_vita_receiver_0_io_vita_clk_out_n_pin : in std_logic;
      fmc_imageon_vita_receiver_0_io_vita_sync_p_pin : in std_logic;
      fmc_imageon_vita_receiver_0_io_vita_sync_n_pin : in std_logic;
      fmc_imageon_vita_receiver_0_io_vita_data_p_pin : in std_logic_vector(7 downto 0);
      fmc_imageon_vita_receiver_0_io_vita_data_n_pin : in std_logic_vector(7 downto 0)
    );
  end component;

  attribute BOX_TYPE : STRING;
  attribute BOX_TYPE of system : component is "user_black_box";

begin

  system_i : system
    port map (
      processing_system7_0_MIO => processing_system7_0_MIO,
      processing_system7_0_PS_SRSTB => processing_system7_0_PS_SRSTB,
      processing_system7_0_PS_CLK => processing_system7_0_PS_CLK,
      processing_system7_0_PS_PORB => processing_system7_0_PS_PORB,
      processing_system7_0_DDR_Clk => processing_system7_0_DDR_Clk,
      processing_system7_0_DDR_Clk_n => processing_system7_0_DDR_Clk_n,
      processing_system7_0_DDR_CKE => processing_system7_0_DDR_CKE,
      processing_system7_0_DDR_CS_n => processing_system7_0_DDR_CS_n,
      processing_system7_0_DDR_RAS_n => processing_system7_0_DDR_RAS_n,
      processing_system7_0_DDR_CAS_n => processing_system7_0_DDR_CAS_n,
      processing_system7_0_DDR_WEB_pin => processing_system7_0_DDR_WEB_pin,
      processing_system7_0_DDR_BankAddr => processing_system7_0_DDR_BankAddr,
      processing_system7_0_DDR_Addr => processing_system7_0_DDR_Addr,
      processing_system7_0_DDR_ODT => processing_system7_0_DDR_ODT,
      processing_system7_0_DDR_DRSTB => processing_system7_0_DDR_DRSTB,
      processing_system7_0_DDR_DQ => processing_system7_0_DDR_DQ,
      processing_system7_0_DDR_DM => processing_system7_0_DDR_DM,
      processing_system7_0_DDR_DQS => processing_system7_0_DDR_DQS,
      processing_system7_0_DDR_DQS_n => processing_system7_0_DDR_DQS_n,
      processing_system7_0_DDR_VRN => processing_system7_0_DDR_VRN,
      processing_system7_0_DDR_VRP => processing_system7_0_DDR_VRP,
      IIC_FMC_SCL => IIC_FMC_SCL,
      IIC_FMC_SDA => IIC_FMC_SDA,
      fmc_imageon_hdmi_in_0_io_hdmii_spdif_pin => fmc_imageon_hdmi_in_0_io_hdmii_spdif_pin,
      fmc_imageon_hdmi_out_0_io_hdmio_spdif_pin => fmc_imageon_hdmi_out_0_io_hdmio_spdif_pin,
      fmc_imageon_hdmi_out_0_io_hdmio_video_pin => fmc_imageon_hdmi_out_0_io_hdmio_video_pin,
      fmc_imageon_hdmi_out_0_io_hdmio_clk_pin => fmc_imageon_hdmi_out_0_io_hdmio_clk_pin,
      fmc_imageon_video_clk1 => fmc_imageon_video_clk1,
      fmc_imageon_iic_0_Rst_pin => fmc_imageon_iic_0_Rst_pin,
      fmc_imageon_iic_0_Sda_pin => fmc_imageon_iic_0_Sda_pin,
      fmc_imageon_iic_0_Scl_pin => fmc_imageon_iic_0_Scl_pin,
      BTNs_5Bits_GPIO => BTNs_5Bits_GPIO,
      SWs_8Bits_GPIO => SWs_8Bits_GPIO,
      fmc_imageon_vita_receiver_0_io_vita_clk_pll_pin => fmc_imageon_vita_receiver_0_io_vita_clk_pll_pin,
      fmc_imageon_vita_receiver_0_io_vita_reset_n_pin => fmc_imageon_vita_receiver_0_io_vita_reset_n_pin,
      fmc_imageon_vita_receiver_0_io_vita_trigger_pin => fmc_imageon_vita_receiver_0_io_vita_trigger_pin,
      fmc_imageon_vita_receiver_0_io_vita_monitor_pin => fmc_imageon_vita_receiver_0_io_vita_monitor_pin,
      fmc_imageon_vita_receiver_0_io_vita_spi_sclk_pin => fmc_imageon_vita_receiver_0_io_vita_spi_sclk_pin,
      fmc_imageon_vita_receiver_0_io_vita_spi_ssel_n_pin => fmc_imageon_vita_receiver_0_io_vita_spi_ssel_n_pin,
      fmc_imageon_vita_receiver_0_io_vita_spi_mosi_pin => fmc_imageon_vita_receiver_0_io_vita_spi_mosi_pin,
      fmc_imageon_vita_receiver_0_io_vita_spi_miso_pin => fmc_imageon_vita_receiver_0_io_vita_spi_miso_pin,
      fmc_imageon_vita_receiver_0_io_vita_clk_out_p_pin => fmc_imageon_vita_receiver_0_io_vita_clk_out_p_pin,
      fmc_imageon_vita_receiver_0_io_vita_clk_out_n_pin => fmc_imageon_vita_receiver_0_io_vita_clk_out_n_pin,
      fmc_imageon_vita_receiver_0_io_vita_sync_p_pin => fmc_imageon_vita_receiver_0_io_vita_sync_p_pin,
      fmc_imageon_vita_receiver_0_io_vita_sync_n_pin => fmc_imageon_vita_receiver_0_io_vita_sync_n_pin,
      fmc_imageon_vita_receiver_0_io_vita_data_p_pin => fmc_imageon_vita_receiver_0_io_vita_data_p_pin,
      fmc_imageon_vita_receiver_0_io_vita_data_n_pin => fmc_imageon_vita_receiver_0_io_vita_data_n_pin
    );

end architecture STRUCTURE;

