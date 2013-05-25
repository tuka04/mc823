package biblioteca;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;


public class Usuario {
	private static String Table="usuario";
	private Connection conn=null;/*conexao com o banco*/
	/*necessita de uma conexao com o banco*/
	public Usuario(Connection c){
		this.conn=c;
	}
	/**
	 * Metodo que realiza o login
	 * @param String s : senha
	 * @return boolean : true em caso de sucesso do login
	 */
	public boolean login(String s){
		String qr = "SELECT * FROM "+Usuario.Table+" WHERE senha LIKE '"+s+"'";/*query*/
		Statement st = null;/*statement*/
		ResultSet r = null;/*resultado da query*/
		/*tentativa de criar o statement*/
		try{
			st = this.conn.createStatement();
		}catch(SQLException e){/*algum problema?*/
			System.out.print("Error: getBooks, falha na ao criar statement");/*falha*/
			e.printStackTrace();
			return false;/*fim*/
		}
		/*tentativa de executar a query*/
		try{
			 r = st.executeQuery(qr);
			 if(!r.next())/*senha nao encontrada*/
				 return false;/*login nao realizado*/
			 return true;/*login realizado*/
		}catch(SQLException e){/*algum problema?*/
			System.out.print("Error: getBooks, falha ao executar query");/*erro na query*/
			e.printStackTrace();
			return false;/*fim*/
		}
	}
}
