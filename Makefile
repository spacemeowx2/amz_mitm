KIPS := amz_mitm
NROS := amzmitm_config

SUBFOLDERS := Atmosphere-libs/libstratosphere $(KIPS) $(NROS)

TOPTARGETS := all clean

OUTDIR		:=	out
SD_ROOT     :=  $(OUTDIR)/sd
NRO_DIR     :=  $(SD_ROOT)/switch/amzmitm_config
TITLE_DIR   :=  $(SD_ROOT)/atmosphere/contents/4200000000000012

$(TOPTARGETS): PACK

$(SUBFOLDERS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

$(KIPS): Atmosphere-libs/libstratosphere

#---------------------------------------------------------------------------------
PACK: $(SUBFOLDERS)
	@[ -d $(NRO_DIR) ] || mkdir -p $(NRO_DIR)
	@[ -d $(TITLE_DIR)/flags ] || mkdir -p $(TITLE_DIR)/flags
	@ cp amzmitm_config/amzmitm_config.nro $(NRO_DIR)/amzmitm_config.nro
	@ cp amz_mitm/amz_mitm.nsp $(TITLE_DIR)/exefs.nsp
	@ touch $(TITLE_DIR)/flags/boot2.flag
#---------------------------------------------------------------------------------

.PHONY: $(TOPTARGETS) $(SUBFOLDERS)
