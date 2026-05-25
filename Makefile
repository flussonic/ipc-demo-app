TOOLCHAIN ?=
TOOLCHAIN_PREFIX ?= arm-linux-
DOCKER_IMAGE ?= ipc-demo-app

BUILD ?= build
TARGET ?= $(BUILD)/http_200

.PHONY: all check docker-build docker-artifact

all: docker-build docker-artifact check

check:
	readelf -h $(TARGET)

docker-build:
ifeq ($(TOOLCHAIN),)
	docker build -t $(DOCKER_IMAGE) .
else
	test -f "$(TOOLCHAIN)"
	docker build -f Dockerfile.toolchain \
		--build-arg TOOLCHAIN_ARCHIVE="$(TOOLCHAIN)" \
		--build-arg TOOLCHAIN_PREFIX="$(TOOLCHAIN_PREFIX)" \
		-t $(DOCKER_IMAGE) .
endif

docker-artifact: docker-build
	mkdir -p $(BUILD)
	id=$$(docker create $(DOCKER_IMAGE)); \
	docker cp $$id:/src/$(TARGET) $(TARGET); \
	docker rm $$id
