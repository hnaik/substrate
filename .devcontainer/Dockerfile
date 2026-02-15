FROM gcc:latest

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update -y -qq && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y -qq \
    binutils \
    git-core \
    htop \
    locales \
    ninja-build \
    python3 \
    python3-dev \
    sysvinit-utils \
    sudo \
    tzdata \
    zsh && apt-get autoremove -y && \
    apt-get clean -y && \
    rm -rf /var/lib/apt/lists/*

RUN locale-gen en_US.UTF-8 && dpkg-reconfigure locales

RUN curl -fsSL https://pixi.sh/install.sh | bash -s -- && \
    mv /root/.pixi/bin/pixi /usr/local/bin/

ENV HOSTNAME="substrate-dev-container"
RUN echo $HOSTNAME > /etc/hostname

ARG USERNAME=sbsdev
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# Create the user
RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    #
    # [Optional] Add sudo support. Omit if you don't need to install software after connecting.
    && apt-get update \
    && apt-get install -y sudo \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

# ********************************************************
# * Anything else you want to do like clean up goes here *
# ********************************************************

# [Optional] Set the default user. Omit if you want to keep the default as root.
USER $USERNAME
