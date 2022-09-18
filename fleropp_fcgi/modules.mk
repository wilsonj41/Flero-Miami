mod_fleropp_fcgi.la: mod_fleropp_fcgi.slo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  mod_fleropp_fcgi.lo
DISTCLEAN_TARGETS = modules.mk
shared =  mod_fleropp_fcgi.la
