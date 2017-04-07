public class Vt6main
{
	public static void main (String [] args)
	{
		Sanalista ens_lista = new Sanalista("Sanoja.txt"); // Sanalista konstruktorin parametriksi annetaan se tiedosto, jossa mahdolliset arvattavaksi valittavat sanat ovat
		Sanalista toinen_lista = ens_lista.sanatJoidenPituusOn(6);
		Sanalista kolmas_lista = ens_lista.sanaJoissaMerkit("_i______");
		
		Ikkuna peli_1 = new Ikkuna(new Hirsipuu(ens_lista, 10));
		//Ikkuna peli_2 = new Ikkuna(new Hirsipuu(toinen_lista, 10));
		//Ikkuna peli_3 = new Ikkuna(new Hirsipuu(kolmas_lista, 10));
	}
}