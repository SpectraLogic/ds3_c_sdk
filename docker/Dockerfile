FROM ubuntu:latest

RUN apt-get update

RUN apt-get install -y vim

RUN apt-get install -y build-essential

RUN apt-get install -y libxml2-dev

RUN apt-get install -y libcurl4-openssl-dev

RUN apt-get install -y libglib2.0-dev

RUN apt-get install -y autoconf

RUN apt-get install -y git

RUN apt-get install -y cmake

RUN apt-get install -y libtool

RUN apt-get install -y curl

RUN apt-get install -y libboost-test-dev

RUN apt-get install -y valgrind

RUN gpg --keyserver hkp://keys.gnupg.net --recv-keys 409B6B1796C275462A1703113804BB82D39DC0E3

RUN \curl -sSL https://get.rvm.io | bash -s stable

RUN /bin/bash -l -c "rvm requirements"
RUN /bin/bash -l -c "rvm install 2.2.0"
RUN /bin/bash -l -c "gem install rake --no-ri --no-rdoc"

RUN /bin/bash -l -c "curl -Lo- https://bit.ly/janus-bootstrap | bash"

RUN echo "export TERM=xterm-256color" >> ~/.bashrc

RUN echo "color ir_blue" >> ~/.vimrc.after

ADD run_tests.sh /opt/

ENTRYPOINT ["/opt/run_tests.sh"]

