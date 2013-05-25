package client;
import java.io.IOException;
import java.rmi.Naming; 
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.util.Vector;
import java.util.Random;
import common.*;
import biblioteca.*;
public class Tester {
	
	private static Vector<String> opt;/*opcoes do menu*/
	private static Vector<String> isbn;/*lista com alguns isbn*/
	private static String senha="mc823";/*senha, sempre correta*/
	private static int Total = 100;/*mesmo numero de testes para TCP e UDP*/
	/**
	 * Metodo que inicializa as variasveis do teste
	 */
	private static void init(){
		Tester.opt = new Vector<String>();
		Tester.isbn = new Vector<String>();
		Tester.opt.add(Menu.ListIsbn);
		Tester.opt.add(Menu.DescByIsbn);
		Tester.opt.add(Menu.BookByIsbn);
		Tester.opt.add(Menu.ListBooks);
		Tester.opt.add(Menu.EditBook);
		Tester.opt.add(Menu.AmountByIsbn);
		Tester.isbn.add("081297215-5");
		Tester.isbn.add("207036002-4");
		Tester.isbn.add("843760494-X");
		Tester.isbn.add("850109104-9");
		Tester.isbn.add("857302773-8");
		Tester.isbn.add("972011011-3");
		Tester.isbn.add("978052007-4");
		Tester.isbn.add("978853650-2");
		Tester.isbn.add("978857522-3");
		Tester.isbn.add("978857608-5");
		Tester.isbn.add("978857679-9");
		Tester.isbn.add("978858041-1");
	}
	
	public static void main(String[] args) {
		Tester.init();
		Biblioteca b=null;
		Random random = new Random();
		try { 
			b = (Biblioteca)Naming.lookup("rmi://"+CommonVars.addr+":"+CommonVars.port+"/"+CommonVars.appName);
		} 
		catch (Exception e) {
			System.out.println("Error: Lookup server");
		} 
		/*executa testes*/
		try{
			Tempo tc = new Tempo();
			int i;
			for(i=0;i<Tester.Total;i++){
				for(String o : Tester.opt){
					if(o.equals(Menu.ListIsbn)){
						tc.start();/*inicia cronometro*/
						System.out.print(b.getAllISBN());
						tc.stop();/*finaliza cronometro*/
						tc.write("./estat/tc_1");/*escreve estatistica*/
					}
					if(o.equals(Menu.DescByIsbn)){
						tc.start();/*inicia cronometro*/
						System.out.print(b.getDescByIsbn(Tester.isbn.get(random.nextInt(Tester.isbn.size()))));
						tc.stop();/*finaliza cronometro*/
						tc.write("./estat/tc_2");/*escreve estatistica*/
					}
					if(o.equals(Menu.BookByIsbn)){
						tc.start();/*inicia cronometro*/
						System.out.print(b.getBookByIsbn(Tester.isbn.get(random.nextInt(Tester.isbn.size()))));
						tc.stop();/*finaliza cronometro*/
						tc.write("./estat/tc_3");/*escreve estatistica*/
					}
					if(o.equals(Menu.ListBooks)){
						tc.start();/*inicia cronometro*/
						System.out.print(b.getBooks());
						tc.stop();/*finaliza cronometro*/
						tc.write("./estat/tc_4");/*escreve estatistica*/
					}
					if(o.equals(Menu.EditBook)){
						tc.start();/*inicia cronometro*/
						b.doLogin(Tester.senha);
						System.out.print(b.setQuantidade(Tester.isbn.get(random.nextInt(Tester.isbn.size())), random.nextInt(1000)));
						tc.stop();/*finaliza cronometro*/
						tc.write("./estat/tc_5");/*escreve estatistica*/
					}
					if(o.equals(Menu.AmountByIsbn)){
						tc.start();/*inicia cronometro*/		
						System.out.print(b.getQuantidade(Tester.isbn.get(random.nextInt(Tester.isbn.size()))));
						tc.stop();/*finaliza cronometro*/
						tc.write("./estat/tc_6");/*escreve estatistica*/
					}
				}
			}
		}
		catch(IOException ioe){
			System.out.print("Error: terminal reader");
			ioe.printStackTrace();
		}
   }
}
