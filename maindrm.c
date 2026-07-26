#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//api DRM
#include <xf86drm.h>
#include <xf86drmMode.h>
//

int main(int argc, char const *argv[])
{
  int fd = open("/dev/dri/card1", O_RDWR);
  if (fd < 0) {
    perror("open");
    return EXIT_FAILURE;
  }
  drmModeRes *resources = drmModeGetResources(fd);
  if (!resources) {
    printf("No se pudieron obtener los recursos DRM.\n");

    close(fd);

    return EXIT_FAILURE;
  }
  printf("Framebuffers : %d\n", resources->count_fbs);

  printf("CRTCs        : %d\n", resources->count_crtcs);

  printf("Connectors   : %d\n", resources->count_connectors);

  printf("Encoders     : %d\n", resources->count_encoders);

  
  

  for (size_t i = 0; i < resources->count_connectors; i++)
  {
    uint32_t id = resources->connectors[i];
    drmModeConnector *conn = drmModeGetConnector(fd, id);
    printf("ID: %u\n", conn->connector_id);
    printf("Modos: %d\n", conn->count_modes);
    switch (conn->connection) {
    case DRM_MODE_CONNECTED:
      printf("Estado: Conectado\n");
      break;

    case DRM_MODE_DISCONNECTED:
      printf("Estado: Desconectado\n");
      break;

    default:
      printf("Estado: Desconocido\n");
    }
    for (size_t i = 0; i < conn->count_modes; i++)
    {
      /* code */
      drmModeModeInfo mode = conn->modes[i];
      printf("modo ancho: %u\n", mode.hdisplay);
      printf("modo alto: %u\n", mode.vdisplay);
      printf("modo refresco: %u\n", mode.vrefresh);
    }
    


    //liberar recursos conn
    drmModeFreeConnector(conn);
  }

  // liberar recursos
  drmModeFreeResources(resources);

  close(fd);

  return 0;
}
