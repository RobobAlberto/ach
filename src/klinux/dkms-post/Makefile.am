
if BUILD_DKMS

install-data-local:
	if test -n "`dkms status ach-klinux/@PACKAGE_VERSION@`"; then \
		dkms remove ach-klinux/@PACKAGE_VERSION@ --all; \
	fi
	dkms add $(prefix)/src/ach-klinux-@PACKAGE_VERSION@
	dkms build -m ach-klinux -v @PACKAGE_VERSION@
	dkms install -m ach-klinux -v @PACKAGE_VERSION@

endif

uninstall-local:
	if test -d $(prefix)/src/ach-klinux-@PACKAGE_VERSION@/ach; then \
		rmdir $(prefix)/src/ach-klinux-@PACKAGE_VERSION@/ach; \
	fi
	if test -d $(prefix)/src/ach-klinux-@PACKAGE_VERSION@; then \
		rmdir $(prefix)/src/ach-klinux-@PACKAGE_VERSION@; \
	fi
