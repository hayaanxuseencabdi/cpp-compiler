FROM gcc:13

RUN apt-get update && apt-get install -y \
    wget \
    fish \
    git \
    g++ \
    cmake \
    ripgrep \
    unzip \
    sudo \
    && rm -rf /var/lib/apt/lists/*

ARG USERNAME=hayaan
RUN useradd -ms /bin/fish $USERNAME

WORKDIR /home/$USERNAME/dev-tools
RUN wget https://github.com/neovim/neovim/releases/download/stable/nvim-linux64.tar.gz
RUN tar xzvf nvim-linux64.tar.gz
RUN rm nvim-linux64.tar.gz
RUN ln -s /home/$USERNAME/dev-tools/nvim-linux64/bin/nvim /usr/local/bin/nvim

USER $USERNAME

CMD [ "/bin/fish" ]
