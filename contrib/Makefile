
LIBOAUTH_CONFIGUREFLAGS := --prefix=$(PWD)/trash --libdir=$(PWD)/../libs/

.PHONY: liboauth-1.0.1

liboauth-1.0.1: liboauth-1.0.1/config.status
	$(MAKE) -C liboauth-1.0.1
	$(MAKE) -C liboauth-1.0.1 install
	rm -vr $(PWD)/trash

liboauth-1.0.1/config.status: liboauth-1.0.1.tar.gz
	tar -xzf liboauth-1.0.1.tar.gz
	cd liboauth-1.0.1 && ./configure $(LIBOAUTH_CONFIGUREFLAGS)

