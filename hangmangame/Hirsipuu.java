import java.util.Random;
import java.util.ArrayList;
import java.util.List;
import java.util.Iterator;
import java.util.Arrays;

public class Hirsipuu
{
	private String arvattava_sana;
	private String peitetty_sana;
	private int arvausten_maara = 0;
	private List<Character> arvauksia;
	
	public Hirsipuu (final Sanalista lista, final int arv_maara)
	{
		Random rgen = new Random();
		int indeksi;
		try
		{
			indeksi = rgen.nextInt(lista.annaSanat().size()); // Arvotaan sellainen numero, joka määrää sen sanan, joka valitaan lista-parametrin sanoja-attribuutista
			this.arvattava_sana = lista.annaSanat().get(indeksi);
			this.peitetty_sana = tiettyString(this.sana().length());
		}catch(IllegalArgumentException iae)
		{
			System.out.println("Ei ole sanoja, joita voisi arpoa arvattavaksi sanaksi.");
		}
		this.setMaara(arv_maara);
		this.arvauksia = new ArrayList<>();
	}
	
	public Hirsipuu(final Hirsipuu puu_par)
	{
		this.setSana(puu_par.sana());
		this.setPeitettySana(puu_par.getPeitettySana());
		this.setMaara(puu_par.arvauksiaOnJaljella());
		this.setArvaukset(puu_par.arvaukset());
	}
	
	public void setSana(final String sana_par)
	{
		this.arvattava_sana = sana_par;
	}
	
	public void setPeitettySana(final String sana_par)
	{
		this.peitetty_sana = sana_par;
	}
	
	public void setMaara(final int maara_par)
	{
		if(maara_par >= 0)
			this.arvausten_maara = maara_par;
		else
			System.out.println("Arvausten m\u00E4\u00E4r\u00E4 ei voi olla negatiivinen.");
	}
	
	public void setArvaukset(final List<Character> arvauksia_par)
	{
		this.arvauksia = arvauksia_par;
	}
	
	public String sana() // get-metodi attribuutille "arvattava_sana"
	{
		return this.arvattava_sana;
	}
	
	public String getPeitettySana()
	{
		return this.peitetty_sana;
	}
	
	public int arvauksiaOnJaljella() // get-metodi attribuutille "arvausten_maara"
	{
		return this.arvausten_maara;
	}
	
	public List<Character> arvaukset() // get-metodi attribuutille "arvauksia"
	{
		return this.arvauksia;
	}
	
	public boolean arvaa(final Character merkki_par)
	{
		Character merkki = new Character(Character.toLowerCase(merkki_par.charValue())); // Luodaan uusi merkki, joka on pieni kirjain
		if(!this.arvauksetOnLoppu() && !this.onLoppu())
		{
			this.arvauksia.add(merkki);
			if(this.sana().contains(merkki.toString()))
			{
				this.setPeitettySana(uusiSana(merkki));
				return true;
			}
			else
			{
				this.arvausten_maara--;
				return false;
			}
		}
		else // Palautetaan false, jos arvausten määrä on loppunut tai peli on päättynyt
		{
			return false;
		}
	}

	public boolean onLoppu()
	{
		if(this.getPeitettySana().equals(this.sana()))
			return true;
		else
			return false;
	}
	
	public boolean arvauksetOnLoppu()
	{
		if(this.arvauksiaOnJaljella() == 0)
			return true;
		else
			return false;
	}
	
	private static String tiettyString(final int koko) // Palautetaan sellainen String, jonka pituus määräytyy koko-parametrin perusteella ja joka on täytetty merkeillä '_'. Metodia ei ole tarkoitus käyttää muualla, mistä johtuu private-näkyvyys
	{
		char [] taulukko = new char[koko];
		Arrays.fill(taulukko, '_');
		return new String(taulukko);
	}
	
	private String uusiSana(final Character merkki) // Metodia ei ole tarkoitus käyttää muualla, mistä johtuu private-näkyvyys
	{
		StringBuilder uusi = new StringBuilder(this.getPeitettySana());
		for(int i = 0; i < this.getPeitettySana().length(); i++)
		{
			if((uusi.charAt(i) == '_') && (this.sana().charAt(i) == merkki.charValue())) // Jos parametrina annettava merkki löytyy arvattavassa sanassa indeksin "i" kohdalta, niin peitettyynkin sanaan lisätään kyseinen merkki indeksin "i" kohdalle
				uusi.setCharAt(i, merkki.charValue());
		}
		return uusi.toString();
	}
}