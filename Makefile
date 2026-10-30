.PHONY: all project1 project2 project3 project4 test clean

all: project1 project2 project3 project4

project1:
	$(MAKE) -C project1_elf_forensics all

project2:
	$(MAKE) -C project2_syscall_forensics all

project3:
	$(MAKE) -C project3_cpython_extension build test benchmark

project4:
	$(MAKE) -C project4_service_controller all

test: project1 project2 project3 project4

clean:
	$(MAKE) -C project1_elf_forensics clean
	$(MAKE) -C project2_syscall_forensics clean
	$(MAKE) -C project3_cpython_extension clean
	$(MAKE) -C project4_service_controller clean
