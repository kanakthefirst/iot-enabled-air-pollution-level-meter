#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/xdctools_3_62_01_15_core/packages;C:/ti/simplelink_cc32xx_sdk_6_10_00_05/source;C:/ti/simplelink_cc32xx_sdk_6_10_00_05/kernel/tirtos/packages;C:/ti/simplelink_cc32xx_sdk_6_10_00_05/kernel/tirtos7/packages;C:/ti/simplelink_cc32xx_sdk_6_10_00_05
override XDCROOT = C:/ti/xdctools_3_62_01_15_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/xdctools_3_62_01_15_core/packages;C:/ti/simplelink_cc32xx_sdk_6_10_00_05/source;C:/ti/simplelink_cc32xx_sdk_6_10_00_05/kernel/tirtos/packages;C:/ti/simplelink_cc32xx_sdk_6_10_00_05/kernel/tirtos7/packages;C:/ti/simplelink_cc32xx_sdk_6_10_00_05;C:/ti/xdctools_3_62_01_15_core/packages;..
HOSTOS = Windows
endif
