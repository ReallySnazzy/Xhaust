
# does some magic, then runs the batch file in an OSX friendly way

if [ "$1" != "run" ]; then
docker run --rm -it -v "$PWD":/usr/src/xhaust -w /usr/src/xhaust gcc:4.9 sh ./compile.bat
mv build/xhaust.exe build/xhaust
else
docker run --rm -it -v "$PWD":/usr/src/xhaust -w /usr/src/xhaust gcc:4.9 ./build/xhaust $2
fi

# it's hacky, but run `./docker-compile.sh` to compile.
# and `.docker-compile.sh run` to run.

# FAQ

# First compilation may take a long time,
# because the gcc image needs to be downloaded

# If you're running Linux
	# You don't need this script
	# Run `chmod +x compile.bat`
	# Run the "batch" file with `./compile.bat`

# If you're running Windows
	# Why are you trying to run an sh script you dumbass?

# Receiving "bash: docker: command not found" error
	# You must install docker

# Receiving "Cannot connect to the Docker daemon..." error
	# Start the docker daemon
