# TP1-SO

### Pre-requisitos 📋
_Instalar minisat_
```
apt-get install minisat
```

### Compilacion 🛠️
```
make all o make
```
### Ejecución 🚀
_Se debe ejecutar el proceso aplicación primero pasandole como argumento los archivos a procesar. En este caso se colocaron todos los archivos en un directorio llamado "files"_
```
./bin/master files/*
```
_Luego ejecutar el proceso vista, en otra terminal, pasandole como argumento lo que imprime el proceso aplicación por salida estándar o pasarselo luego por entrada estándar_

```
./bin/view shm_id
```
_Otra forma_
```
./bin/master files/* | ./bin/view 
```


## Autores
* **Gaspar Budó Berra**
* **Bruno Squillari**
* **Facundo Zimbimbakis**

