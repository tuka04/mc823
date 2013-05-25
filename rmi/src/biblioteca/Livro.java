/**
 * Classe de manipulação de um livro
 */

package biblioteca;

import java.util.Arrays;

public class Livro {
	private String isbn="";
	private String titulo="";
	private String[] autores={};
	private String desc="";
	private String editora="";
	private String ano="";
	private int quantidade=-1;
	
	public Livro(String isbn, String titulo, String[] autores, String desc, String editora, String ano, int qnt){
		/*atributos*/
		this.isbn=isbn;
		this.titulo=titulo;
		this.autores=autores;
		this.desc=desc;
		this.editora=editora;
		this.ano=ano;
		this.quantidade = qnt;
	}
	/*apenas o isbn*/
	public Livro(String isbn){
		this.isbn=isbn;
		this.quantidade=-1;
	}
	public Livro(String isbn,String desc){
		this.isbn="";
		this.desc=desc;
		this.quantidade=-1;
	}
	public Livro(String isbn,int qnt){
		this.isbn="";
		this.quantidade=qnt;
	}
	public String toString(){
		String s = new String();
		if(this.isbn.length()>0)
			s=s.concat("ISBN: "+this.isbn+"\n");
		if(this.titulo.length()>0)
			s=s.concat("Título: "+this.titulo+"\n");
		if(this.autores.length>0)
			s=s.concat("Autores: "+Arrays.toString(this.autores)+"\n");
		if(this.desc.length()>0)
			s=s.concat("Descrição: "+this.desc+"\n");
		if(this.editora.length()>0)
			s=s.concat("Editora: "+this.editora+"\n");
		if(this.ano.length()>0)
			s=s.concat("Ano: "+this.ano+"\n");
		if(this.quantidade>0)
			s=s.concat("Quantidade: "+this.quantidade+"\n");
		return s;
	}
}
