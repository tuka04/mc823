package biblioteca;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.sql.DriverManager;
import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.ResultSet;
import java.util.Vector;
import common.*;

public class BibliotecaImpl extends UnicastRemoteObject implements Biblioteca {
	private Tempo tp;/*tempo de processamento*/
	private Connection conn=null;/*conexao com o banco*/
	
	public BibliotecaImpl() throws RemoteException {
		super();
		this.tp=new Tempo();
		try{
			Class.forName("com.mysql.jdbc.Driver");/*driver ok?*/
		}catch(ClassNotFoundException e){
			System.out.println("Error: JDBC connector missing");/*falta o driver .jar*/
			e.printStackTrace();
			return;
		}
		try{
			/*registrando a conexão com o banco*/
			this.conn = DriverManager.getConnection("jdbc:mysql://"+CommonVars.Host+":"+CommonVars.Port+"/"+CommonVars.DataBase,CommonVars.User,CommonVars.Password);
		}catch(SQLException e){
			System.out.println("Error: Falha ao tentar conexão com o banco de dados");/*algum problema ao conectar com o banco*/
			e.printStackTrace();
			return;
		}
		if(this.conn==null){/*Essa conexão realmente existe?*/
			System.out.println("Error: Falta conexão com o banco de dados");
		}
	}
	/**
	 * Método que lista todos os ISBN presentes na Biblioteca.
	 * @return String 
	 */
	public String getAllISBN() throws RemoteException {
		this.tp.start();/*inicio do tempo de processamento*/
		String s = new String();/*string de retorno*/
		String qr = "SELECT isbn FROM "+CommonVars.Table+" ORDER BY isbn ASC";/*query*/
		Statement st = null;/*statement*/
		ResultSet r = null;/*resultado da query*/
		Vector<Livro> livros = new Vector<Livro>();/*livros*/
		/*tentativa de criar o statement*/
		try{
			st = this.conn.createStatement();
		}catch(SQLException e){/*algum problema?*/
			System.out.print("Error: getAllISBN, falha na ao criar statement");/*falha*/
			e.printStackTrace();
			return null;/*fim*/
		}
		/*tentativa de executar a query*/
		try{
			 r = st.executeQuery(qr);
			 while(r.next()){/*vamos ver os resultados*/
				 livros.add(new Livro(r.getString("isbn")));/*livro com apenas o isbn*/
			 }
			 for(Livro l : livros)/*percorrer os livros e concaternar à string s*/
				 s=s.concat(l.toString()+"\n");
		}catch(SQLException e){/*algum problema?*/
			System.out.print("Error: getAllISBN, falha ao executar query");/*erro na query*/
			e.printStackTrace();
			return null;/*fim*/
		}
		this.tp.stop();/*fim cronometro*/
		this.tp.write("./estat/tp_1");/*escreve estatistica*/
		if(s.length()==0)/*não existe nenhum livro*/
			return Menu.BookNotFound;/*imprime livro nao encontrado*/
		return s;/*retorna uma string com os isbn*/
	}
	
	/**
	 * Método que lista a descricao de um livro dado um ISBN
	 * @param String isbn;
	 * @return String s;
	 */
	public String getDescByIsbn(String isbn) throws RemoteException {
		this.tp.start();/*inicio do tempo de processamento*/
		String s = new String();/*string de retorno*/
		String qr = "SELECT * FROM "+CommonVars.Table+" WHERE isbn LIKE '"+isbn+"'";/*query*/
		Statement st = null;/*statement*/
		ResultSet r = null;/*resultado da query*/
		Vector<Livro> livros = new Vector<Livro>();/*livros*/
		/*tentativa de criar o statement*/
		try{
			st = this.conn.createStatement();
		}catch(SQLException e){/*algum problema?*/
			System.out.print("Error: getBookByIsbn, falha na ao criar statement");/*falha*/
			e.printStackTrace();
			return null;/*fim*/
		}
		/*tentativa de executar a query*/
		try{
			 r = st.executeQuery(qr);
			 while(r.next()){/*vamos ver os resultados*/
				 livros.add(new Livro(isbn,
						 			  r.getString("desc")));/*livro completo*/
			 }
			 for(Livro l : livros)/*percorrer os livros e concaternar à string s*/
				 s=s.concat(l.toString()+"\n");
		}catch(SQLException e){/*algum problema?*/
			System.out.print("Error: getBookByIsbn, falha ao executar query");/*erro na query*/
			e.printStackTrace();
			return null;/*fim*/
		}
		this.tp.stop();/*fim cronometro*/
		this.tp.write("./estat/tp_2");/*escreve estatistica*/
		if(s.length()==0)/*não existe nenhum livro*/
			return Menu.BookNotFound;/*imprime livro nao encontrado*/
		return s;/*retorna uma string com os isbn*/
	}
	
	/**
	 * Método que lista um livro dado um ISBN
	 * @param String isbn;
	 * @return String s;
	 */
	public String getBookByIsbn(String isbn) throws RemoteException {
		this.tp.start();/*inicio do tempo de processamento*/
		String s = new String();/*string de retorno*/
		String qr = "SELECT * FROM "+CommonVars.Table+" WHERE isbn LIKE '"+isbn+"'";/*query*/
		Statement st = null;/*statement*/
		ResultSet r = null;/*resultado da query*/
		Vector<Livro> livros = new Vector<Livro>();/*livros*/
		/*tentativa de criar o statement*/
		try{
			st = this.conn.createStatement();
		}catch(SQLException e){/*algum problema?*/
			System.out.print("Error: getBookByIsbn, falha na ao criar statement");/*falha*/
			e.printStackTrace();
			return null;/*fim*/
		}
		/*tentativa de executar a query*/
		try{
			 r = st.executeQuery(qr);
			 while(r.next())/*vamos ver os resultados*/
				 livros.add(new Livro(isbn,
						 			  r.getString("titulo"),
						 			  r.getString("autores").split(";"),
						 			  r.getString("desc"),
						 			  r.getString("editora"),
						 			  r.getString("ano"),
						 			  r.getInt("quantidade")));/*livro completo*/
			 for(Livro l : livros)/*percorrer os livros e concaternar à string s*/
				 s=s.concat(l.toString()+"\n");
		}catch(SQLException e){/*algum problema?*/
			System.out.print("Error: getBookByIsbn, falha ao executar query");/*erro na query*/
			e.printStackTrace();
			return null;/*fim*/
		}
		this.tp.stop();/*fim cronometro*/
		this.tp.write("./estat/tp_3");/*escreve estatistica*/
		if(s.length()==0)/*não existe nenhum livro*/
			return Menu.BookNotFound;/*imprime livro nao encontrado*/
		return s;/*retorna uma string com os isbn*/
	}
	
	/**
	 * Método que lista todos os livros
	 * @param String isbn;
	 * @return String s;
	 */
	public String getBooks() throws RemoteException {
		this.tp.start();/*inicio do tempo de processamento*/
		String s = new String();/*string de retorno*/
		String qr = "SELECT * FROM "+CommonVars.Table+" ORDER BY isbn ASC";/*query*/
		Statement st = null;/*statement*/
		ResultSet r = null;/*resultado da query*/
		Vector<Livro> livros = new Vector<Livro>();/*livros*/
		/*tentativa de criar o statement*/
		try{
			st = this.conn.createStatement();
		}catch(SQLException e){/*algum problema?*/
			System.out.print("Error: getBooks, falha na ao criar statement");/*falha*/
			e.printStackTrace();
			return null;/*fim*/
		}
		/*tentativa de executar a query*/
		try{
			 r = st.executeQuery(qr);
			 while(r.next())/*vamos ver os resultados*/
				 livros.add(new Livro(r.getString("isbn"),
						 			  r.getString("titulo"),
						 			  r.getString("autores").split(";"),
						 			  r.getString("desc"),
						 			  r.getString("editora"),
						 			  r.getString("ano"),
						 			  r.getInt("quantidade")));/*livro completo*/
			 for(Livro l : livros)/*percorrer os livros e concaternar à string s*/
				 s=s.concat(l.toString()+"\n");
		}catch(SQLException e){/*algum problema?*/
			System.out.print("Error: getBooks, falha ao executar query");/*erro na query*/
			e.printStackTrace();
			return null;/*fim*/
		}
		this.tp.stop();/*fim cronometro*/
		this.tp.write("./estat/tp_4");/*escreve estatistica*/
		if(s.length()==0)/*não existe nenhum livro*/
			return Menu.BookNotFound;/*imprime livro nao encontrado*/
		return s;/*retorna uma string com os isbn */
	}
	
	/**
	 * Método que edita a quantidade em estoque de um livro
	 * @param String isbn;
	 * @return String s;
	 */
	public String setQuantidade(String isbn, int qnt) throws RemoteException {
		this.tp.start();/*inicio do tempo de processamento*/
		String s = new String("Operação realizada com sucesso.\n");/*string de retorno*/
		if(qnt<0)
			return "Erro: A quantidade deve ser um número natural.\n";
		String qr = "UPDATE "+CommonVars.Table+" SET quantidade = "+qnt+" WHERE isbn LIKE '"+isbn+"'";/*query*/
		Statement st = null;/*statement*/
		/*tentativa de criar o statement*/
		try{
			st = this.conn.createStatement();
		}catch(SQLException e){/*algum problema?*/
			System.out.print("Error: setQuantidade, falha na ao criar statement");/*falha*/
			e.printStackTrace();
			return null;/*fim*/
		} 
		/*tentativa de executar a query*/
		try{
			 st.execute(qr);
		}catch(SQLException e){/*algum problema?*/
			System.out.print("Error: setQuantidade, falha ao executar query");/*erro na query*/
			e.printStackTrace();
			return null;/*fim*/
		}
		this.tp.stop();/*fim cronometro*/
		this.tp.write("./estat/tp_5");/*escreve estatistica*/
		if(s.length()==0)/*não existe nenhum livro*/
			return Menu.BookNotFound;/*imprime livro nao encontrado*/
		return s;/*retorna uma string com os isbn*/
	}
	
	/**
	 * Método que lista a quantidade em estoque de um livro
	 * @param String isbn;
	 * @return String s;
	 */
	public String getQuantidade(String isbn) throws RemoteException {
		this.tp.start();/*inicio do tempo de processamento*/
		/*Atencao para carater de comparacao do protocolo TCP, presente no projeto_1
		 *o start do tempo de processamento da opcao 5 encontra-se no metodo doLogin(*);
		 * */ 
		String s = new String();/*string de retorno*/
		String qr = "SELECT quantidade FROM "+CommonVars.Table+" WHERE isbn = '"+isbn+"'";/*query*/
		Statement st = null;/*statement*/
		ResultSet r = null;/*resultado da query*/
		Vector<Livro> livros = new Vector<Livro>();/*livros*/
		/*tentativa de criar o statement*/
		try{
			st = this.conn.createStatement();
		}catch(SQLException e){/*algum problema?*/
			System.out.print("Error: getBooks, falha na ao criar statement");/*falha*/
			e.printStackTrace();
			return null;/*fim*/
		}
		/*tentativa de executar a query*/
		try{
			 r = st.executeQuery(qr);
			 while(r.next())/*vamos ver os resultados*/
				 livros.add(new Livro(isbn,r.getInt("quantidade")));/*livro com isbn e quantidade*/
			 for(Livro l : livros)/*percorrer os livros e concaternar à string s*/
				 s=s.concat(l.toString()+"\n");
		}catch(SQLException e){/*algum problema?*/
			System.out.print("Error: getBooks, falha ao executar query");/*erro na query*/
			e.printStackTrace();
			return null;/*fim*/
		}
		this.tp.stop();/*fim cronometro*/
		this.tp.write("./estat/tp_6");/*escreve estatistica*/
		if(s.length()==0)/*não existe nenhum livro*/
			return Menu.BookNotFound;/*imprime livro nao encontrado*/
		return s;/*retorna uma string com os isbn*/
	}
	public boolean doLogin(String senha) throws RemoteException {
		this.tp.start();/*inicio do tempo de processamento PARA A OPCAO 5 DO MENU*/
		Usuario u = new Usuario(this.conn);
		return u.login(senha);
	}
	/*método que imprime o menu*/
	public String getMenu() throws RemoteException {
		return Menu.getPrint();
	}
}
