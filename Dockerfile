FROM matrim/cmake-examples:3.10.3

RUN apt-get update && apt-get install -y libncurses5-dev gdb

WORKDIR /usr/src/

CMD ["/bin/bash"]
