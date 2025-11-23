# syntax=docker/dockerfile:1

FROM debian:bookworm AS builder
RUN apt-get update \
    && apt-get install -y --no-install-recommends g++ \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /build
COPY exchange_algo/main.cpp ./
RUN g++ -std=c++17 -O2 main.cpp -o main

FROM python:3.11-slim AS runtime
ENV PYTHONDONTWRITEBYTECODE=1 \
    PYTHONUNBUFFERED=1
WORKDIR /app
RUN apt-get update \
    && apt-get install -y --no-install-recommends tini \
    && rm -rf /var/lib/apt/lists/*
COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt
COPY . .
RUN rm -f utils/exchange/main.exe
COPY --from=builder /build/main utils/exchange/main
RUN chmod +x utils/exchange/main
ENTRYPOINT ["tini", "--"]
CMD ["python", "main.py"]
