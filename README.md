# FL Camera (VST3)

Plugin VST3 que faz **uma única coisa**: mostrar, dentro da janela de plugin do FL Studio, o vídeo do dispositivo que você escolher (webcam, placa de captura, câmera virtual do OBS etc.). Ele não grava nada; você grava a tela com o app que preferir e a janela do plugin aparece com a borda normal do FL.

O áudio passa sem alteração, então pode ser colocado em qualquer canal do Mixer.

## Como usar
1. Abra o plugin num slot de efeito do Mixer.
2. Escolha o dispositivo no menu no topo (use "Atualizar" se plugar algo depois).
3. Redimensione a janela como quiser.
4. **Duplo clique** no plugin esconde/mostra a barra de controles, deixando só o vídeo.
5. O dispositivo escolhido e o tamanho da janela ficam salvos no projeto.

## Como compilar (Windows)
Requisitos: Visual Studio 2022 (carga "Desenvolvimento para desktop com C++"), CMake 3.22+ e Git.

```
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
```

O JUCE é baixado automaticamente. Ao terminar, o `FL Camera.vst3` é copiado para `C:\Program Files\Common Files\VST3`. Se não copiar (falta de permissão), rode o terminal como administrador ou pegue o arquivo em `build\FLCamera_artefacts\Release\VST3`.

No FL Studio: **Options > Manage plugins > Find installed plugins**, e depois ache "FL Camera" na lista de efeitos.

## Observações
- Um dispositivo de vídeo geralmente só pode ser usado por um programa por vez. Se o OBS ou outro app estiver usando a câmera, o plugin pode não conseguir abri-la (e vice-versa).
- Se quiser que o plugin apareça mesmo com a câmera ocupada, use a câmera virtual do OBS como dispositivo.
