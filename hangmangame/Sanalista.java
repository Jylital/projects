import java.util.ArrayList;
import java.util.List;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.Iterator;

public class Sanalista
{
	private List<String> sanoja;

	public Sanalista (final String tiedostonNimi)
	{
		this.sanoja = new ArrayList<>(); // Varataan listalle tilaa
		try(BufferedReader file = new BufferedReader(new InputStreamReader(new FileInputStream(tiedostonNimi))))
		{
			String rivi;
			while((rivi = file.readLine()) != null) // Luetaan, niin kauan, että metodi "readLine" palauttaa null (joka tarkoittaa, että päästiin tiedoston loppuun)
			{
				rivi = rivi.toLowerCase();
				this.sanoja.add(rivi);
			}
		}catch(IOException ioe)
		{
			System.out.println("Virhe lukemisessa.");
			ioe.printStackTrace();
		}
	}
	
	public Sanalista (final List<String> sanoja_par)
	{
		this.setSanat(sanoja_par);
	}
	
	public Sanalista (final Sanalista lista_par)
	{
		this(lista_par.annaSanat());
	}
	
	public void setSanat(final List<String> sanoja_par)
	{
		this.sanoja = new ArrayList<>();
		Iterator<String> iter = sanoja_par.iterator();
		String temp;
		while(iter.hasNext())
		{
			temp = iter.next().toLowerCase();	// Tällä varmistetaan, että kaikki talletetut sanat sisältävät vain pieniä kirjaimia
			this.sanoja.add(temp);
		}
	}
	
	public List<String> annaSanat()
	{
		return this.sanoja;
	}
	
	public Sanalista sanatJoidenPituusOn(final int pituus)
	{
		List<String> uusi_lista = new ArrayList<>(); // Luodaan lista, joka täytetään niillä alkioilla, jotka ovat tietyn pituisia
		Iterator<String> iter = this.annaSanat().iterator();
		String temp;
		while(iter.hasNext())
		{
			temp = iter.next();
			if(temp.length() == pituus)
				uusi_lista.add(temp);
		}
		return new Sanalista(uusi_lista); // Luodaan listan avulla uusi Sanalista-olio, joka palautetaan
	}
	
	public Sanalista sanaJoissaMerkit(final String mjono)
	{
		List<String> uusi_lista = new ArrayList<>(); // Luodaan lista, joka täytetään niillä alkioilla, joissa on tietyt merkit tietyissä kohdissa
		Iterator<String> iter = this.sanatJoidenPituusOn(mjono.length()).annaSanat().iterator(); // Tarkistetaan vain sellaiset sanat, joiden pituus on yhtä suuri kuin merkkijonon "mjono"
		String temp;
		while(iter.hasNext())
		{
			temp = iter.next();
			if(onSamat(mjono.toLowerCase(), temp))
				uusi_lista.add(temp);
		}
		return new Sanalista(uusi_lista); // Luodaan listan avulla uusi Sanalista-olio, joka palautetaan
	}
	
	private boolean onSamat(final String annettu_mjono, final String verrattava_mjono)
	{
		for(int i = 0; i < annettu_mjono.length(); i++) // Ei tarvitse testata pituutta, koska se on tehty jo aiemmin
		{
			if((annettu_mjono.charAt(i) != '_') && (annettu_mjono.charAt(i) != verrattava_mjono.charAt(i)))
				return false;
		}
		return true;
	}
}