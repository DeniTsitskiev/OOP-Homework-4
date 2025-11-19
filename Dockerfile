FROM alpine:latest AS builder

# Устанавливаем зависимости для сборки
RUN apk update && apk add --no-cache \
    cmake \
    make \
    g++ \
    git \
    linux-headers

# Копируем исходный код
WORKDIR /app
COPY . .

# Создаем директорию для сборки и собираем (удаляем старую если есть)
RUN rm -rf build && mkdir build && cd build && \
    cmake .. && make -j4

# Финальный образ
FROM alpine:latest

# Устанавливаем только runtime зависимости
RUN apk update && apk add --no-cache libstdc++

# Копируем собранные бинарники
COPY --from=builder /app/build/main /usr/local/bin/main
COPY --from=builder /app/build/tests /usr/local/bin/tests

# Создаем не-root пользователя
RUN adduser -D appuser
USER appuser

WORKDIR /home/appuser

# Запуск основной программы по умолчанию
CMD ["main"]