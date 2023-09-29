git clone <https://github.com/Patricio0501>
cd biblioteca
gcc main.c funciones_biblioteca.c -o biblioteca
./biblioteca

Funcionando correctamente
1. Registrar punto de interés.
2. Mostrar datos de punto de interés.
3. Eliminar punto de interés.
4. Registrar turista.
5. Agregar lugar favorito.
6. Mostrar turistas por país.
7. Mostrar puntos de interés por tipo.
8. Importar archivo de puntos de interés y de archivo de turistas.
9. Exportar puntos de interés a un archivo y exportar turistas a otro archivo.

Función 1: Registrar punto de interes se hace verificacion a todos los caracteres excepto al horario ya que se asume que este se ingresa bien, es decir "00:00-12:00" y para lo demás se hacer verificación para que no pase el limite de caracteres. Además también se asume que no se va a reingresar nuevamente un punto de interés que haya sido ingresado con anterioridad. 

Función 8: Procurar escribir bien el nombre del archivo.

Contribuciones:

Patricio Cisternas M:
Desarrollo las funciones de eliminar punto de interés, agregar lugar favorito, mostrar puntos de interés por tipo, importar archivo de puntos de interés, importar archivo de turistas, exportar puntos de interés a un archivo y exportar turistas a otro archivo
Autoevaluación: 3 (Aporte excelente)

Ignacio Figueroa B:
Desarrollo las funciones de registrar punto de interés, mostrar datos de punto de interés, registrar turista y mostrar turistas por país, además desarrollo los comentarios del código y la elaboración del README.
Autoevaluación: 2 (Aporte bueno)
