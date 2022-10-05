# Flero++ Portable Docker Development Environment

Everything you need to maintain a consistent development and testing environment. This directory contains the definition files necessary for Docker containers used to build and run the software. 

## Getting Started

You will need to install Docker on your system. [Docker Desktop](https://docs.docker.com/desktop/) is the easiest way to get started on Windows, Linux, or macOS. If installing on Windows, ensure you have [WSL2](https://docs.microsoft.com/en-us/windows/wsl/install) enabled and configured and use the WSL2 backend for Docker. It is also highly recommended that you install [Visual Studio Code](https://code.visualstudio.com/) (and the [Remote - WSL](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-wsl) extension if using Windows).

## Cloning the Repository

If you have not done so already, clone this repository to a location *inside of WSL2*, either through the commandline or through VS Code attached to WSL2.

```bash
git clone git@gitlab.csi.miamioh.edu:2023-capstone/FastCGI_Webserver_Plugin/fastcgi-webserver-plugin-project.git 
```

## Starting the Docker Container

Docker supports two primary methods of storing information about images and their derived containers:

- `Dockerfile`: Defines the process to build a container image.
- `docker-compose.yml`: Specifies how a container should be run from an image (including specifying a build context).

### Example
While inside of `httpd_container/`, run:

```bash
docker-compose up -d --build
```

`-d` daemonizes the docker process, and `--build` forces `docker-compose` to build the image using the Dockerfile.

The image will be built and the container will be started. Visit [http://localhost:8080](https://localhost:8080) to verify that Apache (`httpd`) has started.

In order to access an interactive shell within the container (useful for viewing logfiles), run:

```bash
docker exec -it httpd_container /bin/bash
```

When you are ready to leave the interactive shell, type `exit`.

## Making Changes

`docker-compose.yml` is written in the human-readable YAML format. Keep in mind that YAML scopes are defined by indentation depth.

### Volumes

Inside of `httpd_container/docker-compose.yml`, there are several "`volume`s". These map a directory on the host system to a directory inside of the container.

```yaml
volumes:
  - ./public-html:/var/www/html
  - ./etc/apache2.conf:/etc/apache2/apache2.conf
  - ./etc/override:/etc/apache2/override
  - ../../fleropp_fpm/bin:/opt/fleropp_fpm
```

### Webroot

If you modify a file inside of `public-html/`, Apache will serve the updated version as soon as the file is saved. If you modify `etc/apache2.conf`, Apache must be restarted. This is most easily accomplished by restarting the container:

```bash
docker-compose restart
```
