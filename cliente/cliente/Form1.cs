using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Collections;


namespace cliente
{
    public partial class Form1 : Form
    {
        Socket serv;
        Thread Atender;

        delegate void DelegadoParaEscribir(string mensaje);
        delegate void DelegadoParaTablaConectados(string[] trozos);
        delegate void DelegadoParaCerrarForm();

        int puerto = 50095;

        public Form1()
        {
            InitializeComponent();
            //CheckForIllegalCrossThreadCalls = false;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            lbl_email.Hide();
            tB_email.Hide();
            lbl_iniciado.Hide();
            dgv_conectados.ColumnCount = 2;
            dgv_conectados.Columns[0].HeaderText = "Usuario";
            dgv_conectados.Columns[1].HeaderText = "Invitar";

        }

        public void PonContadorServicios(string texto)
        {
            lbl_contar.Text = texto;
        }
        public void PonUsuarioIniciado(string texto)
        {
            lbl_iniciado.Text = texto;
            lbl_iniciado.Show();
            tB_peticion.Text = texto;
        }
        public void PonTablaConectados(string[] trozos)
        {
            dgv_conectados.Rows.Clear();
            int num = Convert.ToInt32(trozos[1]);
            if (num > 0)
            {
                for (int i = 0; i < num; i++)
                {
                    DataGridViewRow row = new DataGridViewRow();
                    row.CreateCells(dgv_conectados);
                    row.Cells[0].Value = trozos[i + 2];

                    DataGridViewCheckBoxCell checkboxCell = new DataGridViewCheckBoxCell();
                    checkboxCell.Value = false;
                    row.Cells[1] = checkboxCell;

                    dgv_conectados.Rows.Add(row);
                }
            }
        }
        public void CerrarMenuPrincipal()
        {
            this.Close();
        }

        private void AtenderServidor()
        {
            while (true)
            {
                //if (serv.Connected == false)
                //    break;
                byte[] msg2 = new byte[200];
                serv.Receive(msg2);
                string mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                //MessageBox.Show(mensaje);
                string[] trozos = mensaje.Split('/');
                int codigo = Convert.ToInt32(trozos[0]);

                switch (codigo)
                {
                    // -------------------------------CONSULTAS------------------------------------
                    case 1:     //PARTIDAS GANADAS
                        MessageBox.Show(tB_peticion.Text + " ha ganado " + trozos[1] + " partidas");
                        if (trozos[2] == "7")
                        {
                            DelegadoParaEscribir delegado1 = new DelegadoParaEscribir(PonContadorServicios);
                            lbl_contar.Invoke(delegado1, new object[] { trozos[3] });
                        }
                        break;
                    case 2:     //PARTIDAS JUGADAS
                        MessageBox.Show(tB_peticion.Text + " ha jugado " + trozos[1] + " partidas");
                        if (trozos[2] == "7")
                        {
                            DelegadoParaEscribir delegado2 = new DelegadoParaEscribir(PonContadorServicios);
                            lbl_contar.Invoke(delegado2, new object[] { trozos[3] });
                        }
                        break;
                    case 3:     //GANADOR
                        MessageBox.Show("El ganador es " + trozos[1]);
                        if (trozos[2] == "7")
                        {
                            DelegadoParaEscribir delegado2 = new DelegadoParaEscribir(PonContadorServicios);
                            lbl_contar.Invoke(delegado2, new object[] { trozos[3] });
                        }
                        break;
                    case 40:    //EXISTE EL USUARIO
                        if (trozos[1] == "CORRECTO")
                            MessageBox.Show(tB_peticion.Text + " EXISTE");
                        else
                            MessageBox.Show(tB_peticion.Text + " NO EXISTE");
                        if (trozos[2] == "7")
                        {
                            DelegadoParaEscribir delegado1 = new DelegadoParaEscribir(PonContadorServicios);
                            lbl_contar.Invoke(delegado1, new object[] { trozos[3] });
                        }
                        break;


                    // -------------------------------FUNCIONES------------------------------------
                    case 4:     //REGISTRO
                        if (trozos[1] == "CORRECTO")
                            MessageBox.Show("Registro completado");
                        else if (trozos[1] == "YAEXISTE")
                            MessageBox.Show("Este usuario ya figura en la base de datos. Prueba a iniciar sesión");
                        else
                            MessageBox.Show("Registro incorrecto");
                        break;
                    case 5:     //INICIO SESION
                        if (trozos[1] == "INCORRECTO")
                        {
                            MessageBox.Show("Nombre de usuario o contraseña incorrecto");
                            string reg51 = "0/" + tB_nombre.Text;
                            byte[] msg51 = System.Text.Encoding.ASCII.GetBytes(reg51);
                            serv.Send(msg51);
                        }
                        else
                        {
                            MessageBox.Show("Bienvenido, " + tB_nombre.Text);
                            DelegadoParaEscribir delegado5 = new DelegadoParaEscribir(PonUsuarioIniciado);
                            lbl_iniciado.Invoke(delegado5, new object[] { tB_nombre.Text });
                            tB_peticion.Invoke(delegado5, new object[] { tB_nombre.Text });

                            string reg52 = "6/";
                            byte[] msg52 = System.Text.Encoding.ASCII.GetBytes(reg52);
                            serv.Send(msg52);
                        }
                        break;
                    case 6:     //LISTA CONECTADOS
                        DelegadoParaTablaConectados delegado3 = new DelegadoParaTablaConectados(PonTablaConectados);
                        dgv_conectados.Invoke(delegado3, new object[] { trozos });
                        break;
                    case 7:     //SERVICIOS
                        DelegadoParaEscribir delegado = new DelegadoParaEscribir(PonContadorServicios);
                        lbl_contar.Invoke(delegado, new object[] { trozos[1] });
                        break;
                    case 20:    //DAR DE BAJA
                        if (trozos[1] == "INCORRECTO")
                            MessageBox.Show("No se ha podido dar de baja");
                        else if (trozos[1] == "CORRECTO")
                        {
                            MessageBox.Show("Usuario dado de baja correctamente");
                            DelegadoParaCerrarForm delegado200 = new DelegadoParaCerrarForm(CerrarMenuPrincipal);
                            this.Invoke(delegado200);
                        }
                        break;

                    // -------------------------------INVITACIONES------------------------------------
                    case 8:     //RECIBIR
                        string popup = trozos[1] + "te ha invitado a una partida. Te atreves?";
                        var pregunta = MessageBox.Show(popup, tB_nombre.Text, MessageBoxButtons.YesNo);
                        Invoke(new Action(() =>
                        {
                            if (pregunta == DialogResult.Yes)
                            {
                                string resp = "9/" + trozos[2] + "/SI/" + tB_nombre.Text + "/" + trozos[1];
                                MessageBox.Show(resp);
                                byte[] msg = System.Text.Encoding.ASCII.GetBytes(resp);
                                serv.Send(msg);
                            }
                            if (pregunta == DialogResult.No)
                            {
                                string resp = "9/" + trozos[2] + "/NO/" + tB_nombre.Text + "/" + trozos[1];
                                MessageBox.Show(resp);
                                byte[] msg = System.Text.Encoding.ASCII.GetBytes(resp);
                                serv.Send(msg);
                            }
                        }));
                        break;
                    case 10:    //EMPEZAR JUEGO
                        if (trozos[1] == "OK" && trozos[2] == tB_nombre.Text)
                        {
                            string a = "Informacion para el host: " + trozos[3] + " se ha unido correctamente a tu partida";
                            MessageBox.Show(a, tB_nombre.Text);
                        }
                        else if (trozos[1] == "OK" && trozos[2] != tB_nombre.Text)
                        {
                            string a = "Informacion para " + trozos[3] + ", te has unido correctamente a la partida de " + trozos[2];
                            MessageBox.Show(a, tB_nombre.Text);
                        }
                        else if (trozos[1] == "KO" && trozos[2] == tB_nombre.Text)
                        {
                            string a = "Informacion para el host: " + trozos[3] + ", no ha podido unirte a tu partida";
                            MessageBox.Show(a, tB_nombre.Text);
                        }
                        else if (trozos[1] == "KO" && trozos[2] != tB_nombre.Text)
                        {
                            string a = "Informacion para " + trozos[3] + ", no has podido unirte a la partida de " + trozos[2];
                            MessageBox.Show(a, tB_nombre.Text);
                        }
                        break;

                }
            }
        }

        private void btn_registro_Click(object sender, EventArgs e)
        {
            lbl_email.Show();
            tB_email.Show();

            if (tB_email.Text == "")
            {
                MessageBox.Show("Introduzca su email para completar el registro.");
            }
            else
            {
                string reg = "4/" + tB_nombre.Text + "/" + tB_contrasena.Text + "/" + tB_email.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(reg);
                serv.Send(msg);
            }
        }

        private void btn_Iniciar_Click(object sender, EventArgs e)
        {
            IPAddress direc = IPAddress.Parse("192.168.56.102");
            IPEndPoint ipep = new IPEndPoint(direc, puerto);
            serv = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            if (tB_nombre.Text == "" || tB_contrasena.Text == "")
            {
                MessageBox.Show("Introduzca nombre de usuario y contrasena");
            }
            else
            {
                try
                {
                    serv.Connect(ipep);
                    //MessageBox.Show("Conexión de socket establecida");
                }
                catch (SocketException)
                {
                    MessageBox.Show("Error de socket");
                    return;
                }

                ThreadStart ts = delegate { AtenderServidor(); };
                Atender = new Thread(ts);
                Atender.Start();

                string reg = "5/" + tB_nombre.Text + "/" + tB_contrasena.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(reg);
                serv.Send(msg);
            }
        }
        private void btn_Olvidado_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Aun no se ha implementado xd");
        }
        private void btn_salir_Click(object sender, EventArgs e)
        {
            string cod = "0/" + tB_nombre.Text;
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(cod);
            serv.Send(msg);
            //serv.Shutdown(SocketShutdown.Both);
            //serv.Close();
            //Atender.Abort();
            this.Close();
        }
        private void btn_enviar_Click(object sender, EventArgs e)
        {
            //Petición partidas ganadas por el usuario seleccionado (1)
            if (cbx_ganadas.Checked)
            {
                string reg = "1/" + tB_peticion.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(reg);
                serv.Send(msg);
            }

            //Petición de partidas jugadas por el usuario (2)
            if (cbx_jugadas.Checked)
            {
                string reg = "2/" + tB_peticion.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(reg);
                serv.Send(msg);
            }
            //Petición de usuario con más ganadas(3)
            if (cbx_ganador.Checked)
            {
                string reg = "3/";
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(reg);
                serv.Send(msg);
            }
            //Petición si existe el usuario (40)
            if (cbx_existe.Checked)
            {
                string reg = "40/" + tB_peticion.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(reg);
                serv.Send(msg);
            }
            if (cbx_ganadas.Checked == false && cbx_jugadas.Checked == false && cbx_ganador.Checked == false && cbx_existe.Checked == false)
                MessageBox.Show("Error en la petición");
        }
        private void btn_invitar_Click(object sender, EventArgs e)
        {
            int invitados = 0;
            string dest = "";
            string dest2 = "";
            string dest3 = "";
            string reg;
            foreach (DataGridViewRow row in dgv_conectados.Rows)
            {
                if (Convert.ToBoolean(row.Cells[1].Value))
                {
                    invitados = invitados + 1;
                }
            }

            if (invitados > 3)
                MessageBox.Show("Máximo 4 invitaciones");
            else
            {
                foreach (DataGridViewRow row in dgv_conectados.Rows)
                {

                    if (Convert.ToBoolean(row.Cells[1].Value) && Convert.ToString(row.Cells[0].Value) == tB_nombre.Text)
                        MessageBox.Show("No te puedes invitar a ti mismo");

                    else if (Convert.ToBoolean(row.Cells[1].Value) && Convert.ToString(row.Cells[0].Value) != tB_nombre.Text)
                    {
                        if (dest == "")
                            dest = Convert.ToString(row.Cells[0].Value).TrimEnd(' ', '\n');
                        else if (dest != "" && dest2 == "")
                            dest2 = Convert.ToString(row.Cells[0].Value).TrimEnd(' ', '\n');
                        else if (dest != "" && dest2 != "" && dest3 == "")
                            dest3 = Convert.ToString(row.Cells[0].Value).TrimEnd(' ', '\n');
                        else
                            MessageBox.Show("No es fa aixi ...");
                    }
                }
                if (invitados == 1)
                    reg = "8/" + invitados.ToString() + "/" + dest + "/" + tB_nombre.Text;
                else if (invitados == 2)
                    reg = "8/" + invitados.ToString() + "/" + dest + "/" + dest2 + "/" + tB_nombre.Text;
                else if (invitados == 3)
                    reg = "8/" + invitados.ToString() + "/" + dest + "/" + dest2 + "/" + dest3 + "/" + tB_nombre.Text;
                else
                    reg = "Algo esta malament bro";
                MessageBox.Show(reg);
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(reg);
                serv.Send(msg);
            }
        }

        private void btn_baja_Click(object sender, EventArgs e)
        {
            string popup = "Seguro que deseas darte de baja?";
            var pregunta = MessageBox.Show(popup, tB_nombre.Text, MessageBoxButtons.YesNo);
            Invoke(new Action(() =>
            {
                if (pregunta == DialogResult.Yes)
                {
                    string reg = "20/" + tB_nombre.Text + "/" + tB_contrasena.Text;
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(reg);
                    serv.Send(msg);
                }
            }));
        }
    }
}