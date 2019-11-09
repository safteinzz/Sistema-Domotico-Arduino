package pi_grupo_4;


import com.panamahitek.ArduinoException;
import com.panamahitek.PanamaHitek_Arduino;

import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;

/**
 * Ejemplo sobre recepción de datos desde Java hasta Arduino
 *
 * @author Antony García González
 */
public class JavaTXconsola {

    //Se crea una instancia de la librería PanamaHitek_Arduino
    private PanamaHitek_Arduino ino;
    
    public JavaTXconsola() {
    	
    	ino = new PanamaHitek_Arduino();
    	try {
    		ino.arduinoTX("COM3", 9600);
    	} catch(ArduinoException ex) {
    		Logger.getLogger(JavaTXconsola.class.getName()).log(Level.SEVERE, null, ex);
    	}
    	
    }
//    private static final SerialPortEventListener listener = new SerialPortEventListener() {
//        @Override
//        public void serialEvent(SerialPortEvent spe) {
//            try {
//                if (ino.isMessageAvailable()) {
//                    //Se imprime el mensaje recibido en la consola
//                    System.out.println(ino.printMessage());
//                }
//            } catch (SerialPortException | ArduinoException ex) {
//                Logger.getLogger(JavaRX.class.getName()).log(Level.SEVERE, null, ex);
//            }
//
//        }
//    };
//1
    public static void main(String[] args) {
    	JavaTXconsola tx = new JavaTXconsola();
    	Scanner sc = new Scanner(System.in);
    	int option = 0;
    	
    	while (option != 2) {
    		System.out.println("¿Que desea hacer?");
    		System.out.println("1) Encender 0) Apagar 2) Salir");
    		
    		option = sc.nextInt();
    		
    		try {
                tx.ino.sendData(option+"");
                
            } catch (ArduinoException | SerialPortException ex) {
                ex.getStackTrace();
            }
    		
    	}
		System.out.println("vuelva pronto");
    	
        
    }
}