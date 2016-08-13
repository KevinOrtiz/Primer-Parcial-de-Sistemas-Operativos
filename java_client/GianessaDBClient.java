import java.net.Socket;
import java.net.InetAddress;
import java.lang.Exception;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.util.LinkedList;
import java.util.ListIterator;

public class GianessaDBClient{
	
	private Socket socket;
	private String ipAdress;
	private int portNumber;
	private final int CHUNK_LENGTH = 6001;

	public GianessaDBClient(String ipAdress, int portNumber){
		this.ipAdress =  ipAdress;
		this.portNumber = portNumber; 
		
	}

	public int connect(){	
		try{
			socket = new Socket(ipAdress, portNumber); //creando el socket	
			InputStream input = socket.getInputStream();//esperando respuesta del servidor
			
			DataInputStream data = new DataInputStream(input);
        	byte [] respond = new byte[12];
        	int characters = data.read(respond,0, 12);
        	if(characters<=0){
        		System.out.println("Me llego 0 bytes");
        		return 0;
        	}
        	String respondString = new String(respond);
         	System.out.println(respondString);
         	if(respondString.equals("CONECTION_OK")){
         		System.out.println("Conectado a GianessaDB");
         		return 1; //exitosa coneccion
         	}
         	else{
         		System.out.println("Error: GianessaDB ocupada");
         		return 0; //error
         	}
         	
		}
		catch(Exception e){
			System.out.println("Error al conectar el socket");
			return 0; //Error
		}

	}

	public int get(LinkedList<String> key){
		int valor = send(new String("GET"));
		if(valor == 1){
			System.out.println("Enviando una clave");
			valor = sendStrings(key);
			if(valor == 1){
				return recieveStrings();
			}

		}
		
		return 0;
		
	}

	public int set(LinkedList<String> key, LinkedList<String> value){
		int valor = send(new String("SET"));
		if(valor != 1){
			return 0;
		}
		valor = sendStrings(key);
		if(valor!=1){
			return 0;
		}
		valor = sendStrings(value);
		if(valor!=1){
			return 0;
		}
		//debo recibir OK
		try{
			InputStream input = socket.getInputStream();//esperando respuesta del servidor
			DataInputStream data = new DataInputStream(input);
			byte [] respond = new byte[2];//el servidor responde OK o NK
        	int characters = data.read(respond,0, 2);
        	
        	if(characters<=0){
        		System.out.println("Me llego 0 bytes");
        		return 0;
        	}
        	String respondString = new String(respond);
         	if(respondString.equals("OK")){
         		return 1; //recibi OK
         	}
         	else{
         		return 0; //error
         	}
		}
		catch(Exception e){
			System.out.println("Exception " + e);
			return 0;
		}

	}

	public int del(LinkedList<String> key){
		int valor = send(new String("DEL"));
		if(valor !=1 ){
			return 0;
		}
		valor = sendStrings(key);
		return valor;
	}

	public void disconnect(){
		System.out.println("Cerrando Socket");
		try{
			socket.close();
		}
		catch(Exception e){
			System.out.println("Exception " + e);
		}
	}


	private int send(String command){

		try{
			OutputStream output = socket.getOutputStream(); 
			DataOutputStream dataOutput = new DataOutputStream(output);
			dataOutput.write(command.getBytes(), 0, command.length());//enviar el comando

			//se lee la respuesta del servidor
			InputStream input = socket.getInputStream();//esperando respuesta del servidor
			DataInputStream data = new DataInputStream(input);
			byte [] respond = new byte[2];//el servidor responde OK o NK
        	int characters = data.read(respond,0, 2);
        	
        	if(characters<=0){
        		System.out.println("Me llego 0 bytes");
        		return 0;
        	}
        	String respondString = new String(respond);
         	if(respondString.equals("OK")){
         		return 1; //exitosa coneccion
         	}
         	else{
         		return 0; //error
         	}

		}
		catch(Exception e){
			System.out.println("Exception: " + e);
			return 0;
		}

	}



	//Envia una lista de strings
	private int sendStrings(LinkedList<String> list){
		ListIterator<String> it = list.listIterator();
		int valor;

		while(it.hasNext()){
			String chunk = it.next();
			valor = send(chunk);
			if(valor != 1){
				System.out.println("Error: no se pudo enviar "+ chunk);
				return 0;
			}
		}
		valor  = send(new String("<<<fin_cadena>>>"));

		return valor;
	}

	private int recieveStrings(){
		LinkedList<String> list = new LinkedList<String>();

		while(true){

			try{

				OutputStream output = socket.getOutputStream(); 
				DataOutputStream dataOutput = new DataOutputStream(output);
				//dataOutput.write(command.getBytes(), 0, command.length());//enviar el comando

				//se lee la respuesta del servidor
				InputStream input = socket.getInputStream();//esperando respuesta del servidor
				DataInputStream data = new DataInputStream(input);
				byte [] respond = new byte[CHUNK_LENGTH];//el servidor responde un chunk
	        	int characters = data.read(respond,0, CHUNK_LENGTH);
	        	
	        	if(characters<=0){
	        		String nk = new String("NK");
	        		dataOutput.write(nk.getBytes(), 0, nk.length());//enviar NK
	        		return 0;
	        	}
	        	String ok = new String("OK");
	        	dataOutput.write(ok.getBytes(), 0, ok.length());//enviar OK

	        	String respondString = new String(respond);//transformo los bytes a String
	        	System.out.println(respondString);

	         	if(respondString.contains("<<<fin_cadena>>>")){
	         		break;
	         	}
	         	
	         	list.add(respondString);

			}
			catch(Exception e){
				System.out.println("Exception " + e);
				return 1;
			}

		}
		return 1;

	}



}