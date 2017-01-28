AX220xx RS-232 to WiFi reference design firmware
=======================================

There are two AX220xx RS-232 to WiFi MCPU demo firmware project files,

proj_gs2w\  	this firmware source code with starting address 0K 
		(for source tracking function through DoCD HAD debugger)

proj_gs2w_32k\ 	If the AX220xx based system includes the Boot Loader firmware, AX220xx MCPU sub-system runtime code
	 	should be used this project file. The MCPU firmware source code with starting address 32K
		