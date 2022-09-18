# Flero++ Portable Docker Development Environment

Everything you need to maintain a consistent development and testing environment. This directory contains the definition files necessary for a Docker container running the Apache HTTP Server.

## Getting Started

You will need to install Docker on your system. [Docker Desktop](https://docs.docker.com/desktop/) is the easiest way to get started on Windows, Linux, or macOS. If installing on Windows, ensure you have [WSL2](https://docs.microsoft.com/en-us/windows/wsl/install) enabled and configured and use the WSL2 backend for Docker. It is also highly recommended that you install [Visual Studio Code](https://code.visualstudio.com/) (and the [Remote - WSL](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-wsl) if using Windows).

## Cloning the Repository

If you have not done so already, clone this repository to a location *inside of WSL2*, either through the commandline or through VS Code attached to WSL2.

```bash
git clone git@gitlab.csi.miamioh.edu:linr5/flero.git
```

## Starting the Docker Container

There are two files related to Docker inside `dev/`:

- `Dockerfile`: Defines the process to build a container image.
- `docker-compose.yml`: Specifies how a container should be run from an image.

While inside of `dev/`, run:

```bash
docker-compose up -d --build
```

`-d` daemonizes the docker process, and `--build` forces `docker-compose` to build the image using the Dockerfile.

The image will be built and the container will be started. Visit [http://localhost:8080](https://localhost:8080) to verify that Apache (`httpd`) has started.

## Making Changes

Inside of `docker-compose.yml`, there are two "volumes". These map a directory on the host system to a directory inside of the container.

```yaml
volumes:
  - ./public-html:/var/www/html
  - ./etc/apache2.conf:/etc/apache2/apache2.conf
```

If you modify a file inside of `public-html/`, Apache will serve the updated version as soon as the file is saved. If you modify `etc/apache2.conf`, Apache must be restarted. This is most easily accomplished by restarting the container:

```bash
docker-compose restart
```


