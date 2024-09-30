ETAPA_DIR := etapa$(ETAPA)

zip:
	make -C $(ETAPA_DIR) clean
	tar cvzf $(ETAPA_DIR).tgz -C $(ETAPA_DIR) .
