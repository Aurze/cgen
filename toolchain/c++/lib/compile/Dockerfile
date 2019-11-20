FROM teeks99/clang-ubuntu:8
RUN apt-get update \
	&& apt-get install -yq \
			cmake \
			ninja-build \
			python3-pip \
                        ninja-build \
			sudo
RUN pip3 install conan \
	&& rm -rf /var/lib/apt/lists/*

ADD build.sh /build.sh
#ENTRYPOINT ["/bin/bash"]
ENTRYPOINT ["/build.sh"]
