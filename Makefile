ETAPA_DIR := etapa$(ETAPA)

zip:
	make -C $(ETAPA_DIR) clean
	tar --exclude='./.vscode' --exclude='./tests' --exclude="./tools" -cvzf $(ETAPA_DIR).tgz -C $(ETAPA_DIR) .
