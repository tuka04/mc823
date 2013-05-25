package common;

public class Tempo {
	private double inicio;/*marca inicio*/
	private double fim;/*marca fim*/
	public Tempo(long i, long f){
		this.inicio=i;
		this.fim=f;
	}
	/*Construtor alternativo*/
	public Tempo(){
		this.inicio=0;
		this.fim=0;
	}
	/*marca inicio do cronometro*/
	public void start(){
		Long l = new Long(System.nanoTime());
		this.inicio=l.doubleValue()/1000.00;
	}
	/*para cronometro*/
	public void stop(){
		Long l = new Long(System.nanoTime());
		this.fim=l.doubleValue()/1000.00;
	}
	/**
	 * retorna tempo inicial
	 * @return long inicio
	 */
	public double getInicio(){
		return this.inicio;
	}
	/**
	 * retorna tempo final
	 * @return long fim
	 */
	public double getFim(){
		return this.fim;
	}
	public double getTempo(){
		return this.fim-this.inicio;
	}
	/**
	 * Escreve o tempo decorrido em um arquivo dado
	 * @param String f : caminho do arquivo
	 */
	public void write(String f){
		InOut.writeOut(f, String.valueOf(this.getTempo())+"\n");
	}
}

