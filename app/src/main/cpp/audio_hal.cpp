//
// Created by root on 27/05/2025.
//
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <hardware/hardware.h>
#include <hardware/audio.h>
#include <log/log.h> // Para logs no Android
#include <asm-generic/errno-base.h>

// Estrutura personalizada do dispositivo de áudio
typedef struct {
    audio_hw_device_t device; // Estrutura padrão do Android HAL
    bool is_initialized;      // Flag de inicialização
    int error_count;          // Contador de erros (simulação)
} custom_audio_device_t;

// Função para fechar o dispositivo (obrigatória na HAL)
static int audio_close(hw_device_t* device) {
    custom_audio_device_t* dev = (custom_audio_device_t*)device;
    if (dev) {
        free(dev);
        ALOGD("HAL de áudio liberada com sucesso.");
    }
    return 0;
}

// Função para escrever dados de áudio (simulada)
static int audio_write(audio_hw_device_t* dev, const void* buffer, size_t bytes) {
    custom_audio_device_t* custom_dev = (custom_audio_device_t*)dev;

    if (!custom_dev->is_initialized) {
        ALOGE("Erro: HAL não inicializada!");
        return -ENODEV;
    }

    ALOGD("Processando %zu bytes de áudio.", bytes); // Log no Android
    return bytes; // Retorna o número de bytes "processados"
}

// Função para abrir o dispositivo (registrada no módulo HAL)
static int audio_open(const hw_module_t* module, const char* name, hw_device_t** device) {
    if (strcmp(name, AUDIO_HARDWARE_INTERFACE) != 0) {
        ALOGE("Erro: Interface %s não suportada!", name);
        return -EINVAL;
    }

    custom_audio_device_t* dev = (custom_audio_device_t*)malloc(sizeof(custom_audio_device_t));
    if (!dev) {
        ALOGE("Falha na alocação de memória.");
        return -ENOMEM;
    }

    memset(dev, 0, sizeof(custom_audio_device_t));

    // Configuração da estrutura padrão
    dev->device.common.tag = HARDWARE_DEVICE_TAG;
    dev->device.common.version = AUDIO_DEVICE_API_VERSION_2_0;
    dev->device.common.module = (hw_module_t*)module;
    dev->device.common.close = audio_close;

    dev->device.write = audio_write; // Simulação de escrita

    dev->is_initialized = true; // Considera inicializado após abertura
    *device = (hw_device_t*)dev;

    ALOGD("Dispositivo de áudio inicializado com sucesso.");
    return 0;
}