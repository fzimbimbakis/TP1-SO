# TP1-SO

### Pre-requisitos 📋
_Instalar minisat_
```
apt-get install minisat
```

### Compilacion 🛠️
```
make all
```
### Ejecución 🚀
_Se debe ejecutar el proceso aplicación primero pasandole como argumento los archivos a procesar. En este caso se colocaron todos los archivos en un directorio llamado "test"_
```
find -name "test/*.cnf" | xargs ./bin/master
```
_Luego ejecutar el proceso vista, en otra terminal, pasandole como argumento lo que imprime el proceso aplicación por salida estándar_

```
./bin/view 
```

## Autores
* **Gaspar Budó Berra**
* **Bruno Squillari**
* **Facundo Zimbimbakis**

