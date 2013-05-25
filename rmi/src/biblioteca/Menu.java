package biblioteca;
import java.rmi.Naming;
import common.*;
public class Menu {
	/*opcoes de menu*/
	public static String ListIsbn = "1";
	public static String DescByIsbn = "2";
	public static String BookByIsbn = "3";
	public static String ListBooks = "4";
	public static String EditBook = "5";
	public static String AmountByIsbn = "6";
	public static String EndProgram="\\q";
	public static String RepeatMenu="\\m";
	public static String BookNotFound = "Livro não encontrado";
	
	/*tempo de comunicacao*/
	private Tempo tc;
	public Menu(){ 
		this.tc=new Tempo();/*inicia objeto tempo*/
	}
	/*dirige a escolha do menu*/
	public void chooseMenu(String opt, Biblioteca b){
		String out = new String();
		try { 
			if(opt.equals(Menu.ListIsbn)){
				this.tc.start();/*inicia cronometro*/
				out=b.getAllISBN();/*contato com servidor*/
				this.tc.stop();/*finaliza cronometro*/
				this.tc.write("./estat/tc_1");/*escreve estatistica*/
			}
			else if(opt.equals(Menu.DescByIsbn)){
				this.tc.start();/*inicia cronometro*/
				out=b.getDescByIsbn(InOut.requestIn(InOut.Isbn));/*contato com servidor*/
				this.tc.stop();/*finaliza cronometro*/
				this.tc.write("./estat/tc_2");/*escreve estatistica*/
			}
			else if(opt.equals(Menu.BookByIsbn)){
				this.tc.start();/*inicia cronometro*/
				out=b.getBookByIsbn(InOut.requestIn(InOut.Isbn));/*contato com servidor*/
				this.tc.stop();/*finaliza cronometro*/
				this.tc.write("./estat/tc_3");/*escreve estatistica*/
			}
			else if(opt.equals(Menu.ListBooks)){
				this.tc.start();/*inicia cronometro*/
				out=b.getBooks();/*contato com servidor*/
				this.tc.stop();/*finaliza cronometro*/
				this.tc.write("./estat/tc_4");/*escreve estatistica*/
			}
			else if(opt.equals(Menu.EditBook)){
				this.tc.start();/*inicia cronometro*/
				if(b.doLogin(InOut.requestIn(InOut.Senha))){/*contato com servidor*/
					String is = InOut.requestIn(InOut.Isbn);/*requisicao de input*/
					int iq = Integer.parseInt(InOut.requestIn(InOut.Quantidade));/*requisicao de input*/
					out=b.setQuantidade(is, iq);/*contato com servidor*/
				}
				else{
					out="Senha inválida!\n";
				}
				this.tc.stop();/*finaliza cronometro*/
				this.tc.write("./estat/tc_5");/*escreve estatistica*/
			}
			else if(opt.equals(Menu.AmountByIsbn)){
				this.tc.start();/*inicia cronometro*/
				out=b.getQuantidade(InOut.requestIn(InOut.Isbn));/*contato com servidor*/
				this.tc.stop();/*finaliza cronometro*/
				this.tc.write("./estat/tc_6");/*escreve estatistica*/
			}
			else if(opt.equals(Menu.RepeatMenu)){
				out=Menu.getPrint();
			}
			else{
				out="Opção inválida!\n";/*oops, opcao invalida*/
				out=out.concat(Menu.getPrint());
			}
		} 
		catch (Exception e) {/*algum erro?*/
			System.out.println("Error: Lookup server");
			e.printStackTrace();
		} 
		System.out.print(out);/*imprime resultado*/
	}
	
	public static String getPrint(){
		String s = "*** MENU (tecle \\m para visualizar o menu) *********\n\0";
		s = s.concat("*** (1) Listar ISBN                                *\n\0");
		s = s.concat("*** (2) Ver descricacao por ISBN (entrada: ISBN)   *\n\0");
		s = s.concat("*** (3) Ver info. completa (entrada: ISBN)         *\n\0");
		s = s.concat("*** (4) Ver info. completa (todos os livros)       *\n\0");
		s = s.concat("*** (5) Alterar estoque (apenas adm)               *\n\0");
		s = s.concat("*** (6) Ver quantidade em estoque (entrada: ISBN)  *\n\0");
		s = s.concat("*** (\\q) Fechar conexao e sair                     *\n\0");
		s = s.concat("****************************************************\n\0");
		return s;
	}
}
