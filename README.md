# Pico Coffee Roaster

Mini torrador de café baseado no Raspberry Pi Pico.

Este firmware é capaz de realizar o controle de uma resistência utilizando um triac, além do controle de um motor e de um fan via PWM.
Também estão disponíveis periféricos como displays, termistores NTC, termopares PT0, controle PID, entre outros.

O projeto utiliza o pico-sdk como base.
Além do pico-sdk instalado e configurado no PATH do ambiente, também é necessário instalar o pico-extras.

## Build e load

Para compilar o projeto utilize:

```bash
mkdir -p build
cd build
cmake ..
make -j$(nproc)
```

Para enviar o firmware para o Pico utilize:

```bash
cd build
picotool load pico_roster.uf2 -f
```

Certifique-se de que o `picotool` esteja instalado e disponível no PATH do sistema.

## Dúvidas e Sugestões

Se tiver alguma dúvida ou sugestão, sinta-se à vontade para utilizar a seção de [ISSUES](https://github.com/JN513/pico_coffee_roaster/issues) no GitHub. Contribuições são bem-vindas e todos os [Pull requests](https://github.com/JN513/pico_coffee_roaster/pulls) serão revisados e, se possível, mesclados.

## Contribuição

Se deseja contribuir com o projeto, sinta-se à vontade para fazê-lo. O arquivo [CONTRIBUTING.md](https://github.com/JN513/pico_coffee_roaster/blob/main/CONTRIBUTING.md) contém as instruções necessárias.

## Licença

Este projeto é licenciado sob a licença MIT]([https://github.com/JN513/pico_coffee_roaster/blob/main/LICENSE](https://github.com/JN513/pico_coffee_roaster/blob/main/LICENSE)), que concede total liberdade para uso.
