import java.util.LinkedList;

public class Main{
	
	public static void main(String [] args){
		GianessaDBClient cl = new GianessaDBClient("127.0.0.1", 8889);
		int valor = cl.connect();
		if(valor == 1){
			String chunk1 = new String("Clave1");
			LinkedList<String> key = new LinkedList<String>();
			key.add(chunk1);

			String chunk2 = new String("Valor 1");
			LinkedList<String> value = new LinkedList<String>();
			value.add(chunk2);

			cl.set(key, value);
			cl.get(key);
			cl.del(key);
			cl.disconnect();
		}

	}

}