segfit:
	$(MAKE) -C src

.PHONY: clean
clean:
	rm ./segfit
	@$(MAKE) clean -C src
