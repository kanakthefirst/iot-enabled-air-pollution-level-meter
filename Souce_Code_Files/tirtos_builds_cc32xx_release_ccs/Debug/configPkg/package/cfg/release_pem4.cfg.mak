# invoke SourceDir generated makefile for release.pem4
release.pem4: .libraries,release.pem4
.libraries,release.pem4: package/cfg/release_pem4.xdl
	$(MAKE) -f C:\Users\kanak\Desktop\iot\tirtos_builds_cc32xx_release_ccs/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\kanak\Desktop\iot\tirtos_builds_cc32xx_release_ccs/src/makefile.libs clean

