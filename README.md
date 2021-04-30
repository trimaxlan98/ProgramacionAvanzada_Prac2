# ProgramacionAvanzada_Prac2
Programa con semaforos que simula una estetica

En una estetica, se tiene a 6 empleados
A la estetica puede llegar cualquier numero de clientes
Los clientes que lleguen pueden solicitar cualquiera de los servicios e incluso todos.
Es importante poder sincronizar tanto a empleados como clientes para que todo fluya correctamente

De los empleados, dos de ellos se encargan de cortar el cabello, dos de ellos realizan la pintura del cabello, 
uno realiza el trabajo de manicure y otro se encarg de hacer la pedicura a los clientes que lo soliciten.

La estetica cuenta con 5 sillas para que los clientes esperen su turno.
Conforme los clientes van llegando a la estetica, toman un asiento.

Todos los clientes van a estar detenidos hasta que los empleados estén listos para poder atender a los clientes.
Pueden estar más de un empleado trabajando.

Si las sillas están ocupadas los demás clientes que lleguen se van a formar de manera que en cuanto se vayan
desocupen las sillas.

El problema debe:
Ser sincronizado cons semáforos
Desplegar el pid de los procesos (tanto de empleados como de clientes)
Indicar lo que en ese momento esta realizando el proceso
