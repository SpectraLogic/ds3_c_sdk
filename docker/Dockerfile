FROM ubuntu:latest

# Combine all apt-get install commands or Docker will attempt to use cache
RUN apt-get update && apt-get install -y \
        autoconf \
        build-essential \
        cmake \
        curl \
        git \
        libboost-test-dev \
        libcurl4-openssl-dev \
        libglib2.0-dev \
        libtool \
        libxml2-dev \
        pkg-config \
        valgrind \
        vim

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

