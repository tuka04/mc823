package common;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.IOException;
import java.io.FileWriter;
import java.io.InputStreamReader;
import java.io.PrintStream;

public class InOut {
	
	public static String Isbn="Isbn:";/*requisicao de isbn*/
	public static String Senha="Senha:";/*requisicao de senha*/
	public static String Quantidade="Quantidade:";/*requisicao de quantidade*/
	
	public static String requestIn(String t){
		if(t.equals(InOut.Isbn)){
			BufferedReader buff = new BufferedReader(new InputStreamReader(System.in));
			System.out.print(InOut.Isbn);
			try{
				return buff.readLine();
			}
			catch(IOException ioe){
				System.out.print("Error: terminal reader");
				ioe.printStackTrace();
			}
		}
		if(t.equals(InOut.Senha)){
			BufferedReader buff = new BufferedReader(new InputStreamReader(System.in));
			System.out.print(InOut.Senha);
			try{
				return buff.readLine();
			}
			catch(IOException ioe){
				System.out.print("Error: terminal reader");
				ioe.printStackTrace();
			}
		}
		if(t.equals(InOut.Quantidade)){
			BufferedReader buff = new BufferedReader(new InputStreamReader(System.in));
			System.out.print(InOut.Quantidade);
			try{
				return buff.readLine();
			}
			catch(IOException ioe){
				System.out.print("Error: terminal reader");
				ioe.printStackTrace();
			}
		}
		return null;
	}
	/**
	 * Escreve string em um arquivo
	 * @param String f : caminho do arquivo
	 * @param String d : dado
	 */
	public static void writeOut(String f, String d){
		try {
			File arq = new File(f);
			if (!arq.exists()) {
				arq.createNewFile();
			}
	        BufferedWriter out = new BufferedWriter(new FileWriter(arq.getAbsoluteFile(),true));
	        out.write(d);
	        out.close();
	    } catch (IOException e) {
	    	System.out.print("Error: writeOut, erro ao escrever em arquivo"+f);
	    	e.printStackTrace();
	    }
	}
	/**
	 * Escreve int em um arquivo
	 * @param String f : caminho do arquivo
	 * @param int d : dado
	 */
	public static void writeOut(String f, int d){
		try {
			File arq = new File(f);
			if (!arq.exists()) {
				arq.createNewFile();
			}
	        BufferedWriter out = new BufferedWriter(new FileWriter(arq.getAbsoluteFile(),true));
	        out.write(d);
	        out.close();
	    } catch (IOException e) {
	    	System.out.print("Error: writeOut, erro ao escrever em arquivo"+f);
	    	e.printStackTrace();
	    }
	}
	/**
	 * Escreve string no bash
	 * @param PrintStream o : bash output
	 * @param String s : dado
	 */
	public static void writeOut(PrintStream o, String s){
		o.println(s);
	}
}
