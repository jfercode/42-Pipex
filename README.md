# 42-Pipex 🚀

42-Pipex es un proyecto que te enseña sobre la comunicación entre procesos utilizando pipes en sistemas Unix. El proyecto consiste en implementar una versión simplificada del comando de shell `|` (pipe), que permite encadenar múltiples comandos.

## ¿Qué es un pipe? ⚙️

Un pipe es un mecanismo para la comunicación entre procesos que permite pasar datos de un proceso a otro en un flujo unidireccional. En sistemas Unix, el operador pipe (`|`) se usa para pasar la salida de un comando como entrada a otro.

El objetivo es implementar un programa que imite el comportamiento del comando de shell `|` dado un teniendo como base un fichero (con permisos de lectura al menos) y indicando un segundo fichero donde va a salir el resultado de la operación.

Aquí están las operaciones que usa el programa:

- **`pipe:`** Crea un pipe y devuelve descriptores de archivo para lectura y escritura.
- **`fork:`** Crea un nuevo proceso duplicando el proceso actual.
- **`dup2:`** Duplica un descriptor de archivo a un descriptor de archivo especificado.
- **`execve:`** Reemplaza la imagen del proceso actual con una nueva imagen de proceso especificada por un archivo.

## Cómo ejecutar 🖥️

Los siguientes comandos deben ser ejecutados en la terminal, dentro del directorio de tu proyecto:

- **`make all`**: Compila el proyecto y genera el ejecutable `pipex`.
- **`make clean`**: Elimina los archivos objeto (`.o`) generados durante la compilación.
- **`make fclean`**: Ejecuta el comando `clean` y, además, elimina el ejecutable `pipex`.
- **`make re`**: Ejecuta los comandos `fclean` y `all`, recompilando todo el proyecto desde cero.

## Uso del programa 🏃

Una vez generado el archivo `pipex`, podrás ejecutarlo pasándole dos comandos como argumentos, teniendo como base un fichero (con permisos de lectura al menos). Algunos posibles ejemplos para su ejecución serían:

```sh
./pipex infile.txt "ls -l" "wc -w" out.txt

./pipex infile "grep a" "wc -w" out.txt

./pipex infile.ber "sleep 1" "sleep 2" out.txt
```

### Casos de error ⚠️

Ten en cuenta que el programa devolverá Error si se topa con uno de los siguientes casos:
- Si alguno de los comandos no existen: `./pipex infile.txt "error_cmd" "ls -l" out.txt`
- Si no se mandan como parametros (y su orden cambia) el archivo de entrada, el cmd1, el cmd2 y el archivo de salida: `./pipex "ls -l" "wc -w" out.txt`
- Hay otros errores como que la ruta absoluta (`$PATH`) esta unseteada o trabaja sin el environment: `env -i ./pipex infile.ber "sleep 1" "sleep 2" out.txt`

## Configuración ⚙️

Puedes ajustar las opciones de compilación en el Makefile según tus necesidades específicas. 

## Autor 👨‍💻
  <br/>
  <br/>
  <br/>

</div>
<div align="center">
  <img src="https://avatars.githubusercontent.com/u/102600920?v=4" alt="Logo" width="200"/>
  <br/>
  <br/>
  <div style="margin: 20px 0 30px;">
  <a href="https://github.com/jfercode">Javier Fernández Correa</a>
  </div>
</div>
  <br/>
<div align="center">
  <img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTVInHuUPtp3uiEuvF0aYAkFBUzpnr65b2CDA&s" alt="Logo"/>
</div>
<br/>
</div>
