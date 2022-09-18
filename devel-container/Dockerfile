# syntax=docker/dockerfile:1.3-labs
FROM ubuntu/apache2:latest
RUN <<-EOF 
    apt-get update
    apt-get install -y build-essential libapache2-mod-fcgid apache2-dev libfcgi-dev
EOF
