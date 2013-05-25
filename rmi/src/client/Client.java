package client;
import java.io.IOException;
import java.rmi.Naming; 
import java.io.InputStreamReader;
import java.io.BufferedReader;
import common.*;
import biblioteca.*;
public class Client {
	 public static void main(String[] args) {
		Biblioteca b=null;
		String str = new String();
		Menu m = new Menu();
		try { 
			b = (Biblioteca)Naming.lookup("rmi://"+CommonVars.addr+":"+CommonVars.port+"/"+CommonVars.appName);
		} 
		catch (Exception e) {
			System.out.println("Error: Lookup server");
		} 
		try{
			InOut.writeOut(System.out, b.getMenu());
			do{
				BufferedReader buff = new BufferedReader(new InputStreamReader(System.in));
				str = buff.readLine();
				m.chooseMenu(str, b);
			}while(!str.equals(Menu.EndProgram));
		}
		catch(IOException ioe){
			System.out.print("Error: terminal reader");
			ioe.printStackTrace();
		}
   }
}
