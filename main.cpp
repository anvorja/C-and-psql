/*
 * ejecutar con g++ -o main main.cpp -lpqxx -lpq
 */
#include <iostream>
#include <pqxx/pqxx>
using namespace std;

int main() {
    try {
        // Establecer la conexión a la base de datos
        pqxx::connection connection("dbname=estudiantes_notas user=postgres password=superapostgres host=localhost port=5432");

        if (connection.is_open()) {
            cout << "Conexión exitosa a la base de datos." << std::endl;
        } else {
            cout << "Error al conectar a la base de datos." << std::endl;
            return 1;
        }

        // Crear una transacción
        pqxx::work transaction(connection);

        // Verificar si la tabla ya existe
        pqxx::result exists = transaction.exec(
                "SELECT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_name = 'estudiantes');");

        bool table_exists = exists[0][0].as<bool>();

        if (!table_exists) {
            // Crear la tabla solo si no existe
            transaction.exec("CREATE TABLE estudiantes (ID serial primary key, Nombre varchar(255), cedula varchar(255));");
        }

        // Insertar algunos datos en la tabla
        transaction.exec("INSERT INTO estudiantes (Nombre, cedula) VALUES ('Juan Perez', '123456789');");
        transaction.exec("INSERT INTO estudiantes (Nombre, cedula) VALUES ('Maria Lopez', '987654321');");
        transaction.exec("INSERT INTO estudiantes (Nombre, cedula) VALUES ('Erick Gomez', '4344343');");
        transaction.exec("INSERT INTO estudiantes (Nombre, cedula) VALUES ('Andres Borja', '545454');");
        transaction.exec("INSERT INTO estudiantes (Nombre, cedula) VALUES ('Alvaro Ramos', '999909');");

        // Realizar commit de la transacción
        transaction.commit();

        cout << "Tabla creada y datos insertados correctamente." << endl;

    } catch (const exception &e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
