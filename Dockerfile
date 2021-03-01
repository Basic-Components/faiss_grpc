FROM --platform=$TARGETPLATFORM python:3.8-slim-buster as install_requirements
RUN sed -i 's/deb.debian.org/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
RUN apt update -y && apt install -y --no-install-recommends build-essential && rm -rf /var/lib/apt/lists/*
COPY pip.conf /etc/pip.conf
RUN pip --no-cache-dir install --upgrade pip
WORKDIR /code
RUN mkdir /code/app
COPY requirements.txt /code/requirements.txt
RUN python -m pip --no-cache-dir install -r requirements.txt --target app
# RUN rm -rf app/*.dist-info

FROM --platform=$TARGETPLATFORM python:3.8-slim-buster as build_img
COPY --from=install_requirements /code/app /code/app/
# 复制源文件
COPY faiss_grpc /code/app/
COPY pyproject.toml /code/pyproject.toml
COPY setup.cfg /code/setup.cfg
COPY setup.py /code/setup.py
RUN python setup.py build_ext
WORKDIR /code
ENTRYPOINT ["python" ,"app"]