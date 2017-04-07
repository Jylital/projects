import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.JButton;
import javax.swing.JOptionPane;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class Ikkuna extends JFrame
{
	private Hirsipuu puu;
	private JPanel ruutu;
	private JLabel viestiLabel_1;
	private JLabel viestiLabel_2;
	private JTextField arvausKentta;
	private JButton vahvistaNappula;
	private JButton lopetaNappula;
	private final int IKKUNAN_LEVEYS = 300;
	private final int IKKUNAN_KORKEUS = 100;
	
	public Ikkuna (final Hirsipuu puu_par)
	{
		this.setPuu(puu_par);
		this.setTitle("Hirsipuu");
		this.setSize(IKKUNAN_LEVEYS, IKKUNAN_KORKEUS);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		rakennaRuutu();
		this.add(ruutu);
		this.setVisible(true);
	}
	
	private void rakennaRuutu() // Näkyvyys on private, sillä kyseistä metodia ei ole tarkoitus käyttää muualla
	{
		this.viestiLabel_1 = new JLabel(this.getPuu().getPeitettySana());
		this.viestiLabel_2 = new JLabel("Anna arvaus: ");
		this.arvausKentta = new JTextField(new JTextFieldLimit(), null, 1); // Tehdään sellainen tekstikenttä, johon ei ole mahdollista syöttää kuin 1 merkki
		
		this.vahvistaNappula = new JButton("Vahvista");
		this.vahvistaNappula.addActionListener(new Kuuntelija());
		
		this.lopetaNappula = new JButton("Lopeta");
		this.lopetaNappula.addActionListener(new Kuuntelija());
		
		this.ruutu = new JPanel();
		
		this.ruutu.add(viestiLabel_1);
		this.ruutu.add(viestiLabel_2);
		this.ruutu.add(arvausKentta);
		this.ruutu.add(vahvistaNappula);
		this.ruutu.add(lopetaNappula);
	}
	
	private class Kuuntelija implements ActionListener
	{
		public void actionPerformed(ActionEvent e)
		{
			String tapahtuma = e.getActionCommand();
			if(tapahtuma.equals("Vahvista"))
			{
				String syote = Ikkuna.this.arvausKentta.getText();
				Character merkki = new Character(syote.charAt(0)); // Syötteeksi valitaan ensimmäinen syötetty kirjain
				if(Ikkuna.this.getPuu().arvaukset().contains(merkki)) // Jos syötteenä annettavaa kirjainta on aikaisemmin jo käytetty arvaamiseen, niin käyttäjä pakotetaan antamaan uusi syöte
				{
					JOptionPane.showMessageDialog(null, "Olet jo k\u00E4ytt\u00E4nyt kyseist\u00E4 kirjainta arvauksena.");
				}
				else if(!Ikkuna.this.getPuu().arvaa(merkki)) // Jos syötteenä annettava kirjain ei löydy arvattavasta sanasta, niin siitä ilmoitetaan käyttäjälle
				{
					if(Ikkuna.this.getPuu().arvauksetOnLoppu()) // Jos arvausten määrä loppuu, niin päätetään ohjelma
					{
						JOptionPane.showMessageDialog(null, "Kirjainta ei l\u00F6ydy arvattavasta sanasta ja arvaustesi m\u00E4\u00E4r\u00E4 on loppunut. Arvattava sana oli \"" + Ikkuna.this.getPuu().sana() + "\".");
						System.exit(0);
					}
					else
						JOptionPane.showMessageDialog(null, "Kirjainta ei l\u00F6ydy arvattavasta sanasta.");	
				}
				else if(!Ikkuna.this.getPuu().onLoppu()) // Jos syötteenä annettava kirjain löytyy arvattavasta sanasta, niin käyttäjälle näkyvää peitettyä sanaa päivitetään
					Ikkuna.this.viestiLabel_1.setText(Ikkuna.this.getPuu().getPeitettySana());
				else // Jos arvattavan sanan kaikki kirjaimet saadaan arvattua, niin käyttäjälle näkyvää peitettyä sanaa päivitetään ja hänelle ilmoitetaan voitosta sekä ohjelma päätetään
				{
					Ikkuna.this.viestiLabel_1.setText(Ikkuna.this.getPuu().getPeitettySana());
					JOptionPane.showMessageDialog(null, "Voitit!");
					System.exit(0);
				}
				Ikkuna.this.arvausKentta.setText(null);
			}
			if(tapahtuma.equals("Lopeta"))
				System.exit(0);
		}
	}

	public void setPuu(final Hirsipuu puu_par)
	{
		this.puu = puu_par;
	}
	
	public Hirsipuu getPuu()
	{
		return this.puu;
	}
}