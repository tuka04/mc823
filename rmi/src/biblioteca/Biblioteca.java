/**
 * Interface da classe Server 
 */
package biblioteca;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Biblioteca extends Remote {
	public String getAllISBN() throws RemoteException;
	public String getDescByIsbn(String isbn) throws RemoteException;
	public String getBookByIsbn(String isbn) throws RemoteException;
	public String getBooks() throws RemoteException;
	public String setQuantidade(String isbn, int qnt) throws RemoteException;
	public String getQuantidade(String isbn) throws RemoteException;
	public String getMenu() throws RemoteException; 
	public boolean doLogin(String senha) throws RemoteException;
}
