compile: 
	arduino-cli compile --fqbn arduino:sam:arduino_due_x_dbg

upload: compile
	arduino-cli upload
clean:
	rm -rf build
