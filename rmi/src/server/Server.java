package server;
import java.rmi.Naming; 
import java.rmi.registry.LocateRegistry;
import biblioteca.*;
import common.*;
public class Server {
	public Server(){
		try { 
			LocateRegistry.createRegistry(CommonVars.port);
			BibliotecaImpl b = new BibliotecaImpl();
			Naming.rebind("//"+CommonVars.addr+":"+Integer.toString(CommonVars.port)+"/"+CommonVars.appName, b); 
		} catch (Exception e) { 
			System.out.print("Error: start server");/*erro na query*/
			e.printStackTrace();
		}
	}
	public static void main(String args[]) { 
        new Server();
    }
	
	public static String getAddr(){
		return CommonVars.addr;
	}

	public static int getPort(){
		return CommonVars.port;
	}

	public static String getAppName(){
		return CommonVars.appName;
	}
}
