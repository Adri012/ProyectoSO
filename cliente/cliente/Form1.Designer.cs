namespace cliente
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            tB_nombre = new TextBox();
            tB_contrasena = new TextBox();
            tB_email = new TextBox();
            lbl_nombre = new Label();
            lbl_contrasena = new Label();
            lbl_email = new Label();
            btn_registro = new Button();
            btn_Iniciar = new Button();
            btn_Olvidado = new Button();
            lbl_Bienvenido = new Label();
            dgv_conectados = new DataGridView();
            lbl_conectados = new Label();
            btn_salir = new Button();
            lbl_peticiones = new Label();
            cbx_ganadas = new CheckBox();
            cbx_jugadas = new CheckBox();
            cbx_ganador = new CheckBox();
            tB_peticion = new TextBox();
            lbl_contar = new Label();
            btn_enviar = new Button();
            btn_invitar = new Button();
            lbl_iniciado = new Label();
            btn_baja = new Button();
            ((System.ComponentModel.ISupportInitialize)dgv_conectados).BeginInit();
            SuspendLayout();
            // 
            // tB_nombre
            // 
            tB_nombre.Location = new Point(266, 146);
            tB_nombre.Name = "tB_nombre";
            tB_nombre.Size = new Size(235, 31);
            tB_nombre.TabIndex = 0;
            // 
            // tB_contrasena
            // 
            tB_contrasena.Location = new Point(266, 217);
            tB_contrasena.Name = "tB_contrasena";
            tB_contrasena.Size = new Size(235, 31);
            tB_contrasena.TabIndex = 1;
            // 
            // tB_email
            // 
            tB_email.Location = new Point(266, 283);
            tB_email.Name = "tB_email";
            tB_email.Size = new Size(235, 31);
            tB_email.TabIndex = 2;
            // 
            // lbl_nombre
            // 
            lbl_nombre.AutoSize = true;
            lbl_nombre.Location = new Point(65, 149);
            lbl_nombre.Name = "lbl_nombre";
            lbl_nombre.Size = new Size(166, 25);
            lbl_nombre.TabIndex = 3;
            lbl_nombre.Text = "Nombre de usuario";
            // 
            // lbl_contrasena
            // 
            lbl_contrasena.AutoSize = true;
            lbl_contrasena.Location = new Point(65, 217);
            lbl_contrasena.Name = "lbl_contrasena";
            lbl_contrasena.Size = new Size(101, 25);
            lbl_contrasena.TabIndex = 4;
            lbl_contrasena.Text = "Contraseña";
            // 
            // lbl_email
            // 
            lbl_email.AutoSize = true;
            lbl_email.Location = new Point(62, 286);
            lbl_email.Name = "lbl_email";
            lbl_email.Size = new Size(157, 25);
            lbl_email.TabIndex = 5;
            lbl_email.Text = "Correo electrónico";
            // 
            // btn_registro
            // 
            btn_registro.Location = new Point(53, 368);
            btn_registro.Name = "btn_registro";
            btn_registro.Size = new Size(178, 63);
            btn_registro.TabIndex = 6;
            btn_registro.Text = "Registrarse";
            btn_registro.UseVisualStyleBackColor = true;
            btn_registro.Click += btn_registro_Click;
            // 
            // btn_Iniciar
            // 
            btn_Iniciar.Location = new Point(337, 368);
            btn_Iniciar.Name = "btn_Iniciar";
            btn_Iniciar.Size = new Size(164, 63);
            btn_Iniciar.TabIndex = 7;
            btn_Iniciar.Text = "Iniciar Juego";
            btn_Iniciar.UseVisualStyleBackColor = true;
            btn_Iniciar.Click += btn_Iniciar_Click;
            // 
            // btn_Olvidado
            // 
            btn_Olvidado.Location = new Point(210, 457);
            btn_Olvidado.Name = "btn_Olvidado";
            btn_Olvidado.Size = new Size(291, 33);
            btn_Olvidado.TabIndex = 8;
            btn_Olvidado.Text = "He olvidado mi contraseña";
            btn_Olvidado.UseVisualStyleBackColor = true;
            btn_Olvidado.Click += btn_Olvidado_Click;
            // 
            // lbl_Bienvenido
            // 
            lbl_Bienvenido.AutoSize = true;
            lbl_Bienvenido.Font = new Font("Papyrus", 28F, FontStyle.Bold | FontStyle.Italic, GraphicsUnit.Point);
            lbl_Bienvenido.Location = new Point(548, 22);
            lbl_Bienvenido.Name = "lbl_Bienvenido";
            lbl_Bienvenido.Size = new Size(259, 88);
            lbl_Bienvenido.TabIndex = 9;
            lbl_Bienvenido.Text = "TABÚ";
            // 
            // dgv_conectados
            // 
            dgv_conectados.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            dgv_conectados.Location = new Point(554, 201);
            dgv_conectados.Name = "dgv_conectados";
            dgv_conectados.RowHeadersWidth = 62;
            dgv_conectados.RowTemplate.Height = 33;
            dgv_conectados.Size = new Size(385, 250);
            dgv_conectados.TabIndex = 10;
            // 
            // lbl_conectados
            // 
            lbl_conectados.AutoSize = true;
            lbl_conectados.Font = new Font("Bahnschrift SemiBold Condensed", 10F, FontStyle.Bold, GraphicsUnit.Point);
            lbl_conectados.Location = new Point(625, 147);
            lbl_conectados.Name = "lbl_conectados";
            lbl_conectados.Size = new Size(234, 24);
            lbl_conectados.TabIndex = 11;
            lbl_conectados.Text = "Usuarios actualmente conectados";
            // 
            // btn_salir
            // 
            btn_salir.Location = new Point(1167, 504);
            btn_salir.Name = "btn_salir";
            btn_salir.Size = new Size(91, 38);
            btn_salir.TabIndex = 12;
            btn_salir.Text = "Salir";
            btn_salir.UseVisualStyleBackColor = true;
            btn_salir.Click += btn_salir_Click;
            // 
            // lbl_peticiones
            // 
            lbl_peticiones.AutoSize = true;
            lbl_peticiones.Font = new Font("Bahnschrift SemiBold Condensed", 10F, FontStyle.Bold, GraphicsUnit.Point);
            lbl_peticiones.Location = new Point(1042, 150);
            lbl_peticiones.Name = "lbl_peticiones";
            lbl_peticiones.Size = new Size(78, 24);
            lbl_peticiones.TabIndex = 13;
            lbl_peticiones.Text = "Peticiones";
            // 
            // cbx_ganadas
            // 
            cbx_ganadas.AutoSize = true;
            cbx_ganadas.Location = new Point(1013, 198);
            cbx_ganadas.Name = "cbx_ganadas";
            cbx_ganadas.Size = new Size(214, 29);
            cbx_ganadas.TabIndex = 15;
            cbx_ganadas.Text = "Partidas ganadas de ...";
            cbx_ganadas.UseVisualStyleBackColor = true;
            // 
            // cbx_jugadas
            // 
            cbx_jugadas.AutoSize = true;
            cbx_jugadas.Location = new Point(1013, 233);
            cbx_jugadas.Name = "cbx_jugadas";
            cbx_jugadas.Size = new Size(209, 29);
            cbx_jugadas.TabIndex = 16;
            cbx_jugadas.Text = "Partidas jugadas de ...";
            cbx_jugadas.UseVisualStyleBackColor = true;
            // 
            // cbx_ganador
            // 
            cbx_ganador.AutoSize = true;
            cbx_ganador.Location = new Point(1013, 268);
            cbx_ganador.Name = "cbx_ganador";
            cbx_ganador.Size = new Size(235, 29);
            cbx_ganador.TabIndex = 17;
            cbx_ganador.Text = "Jugador con más puntos";
            cbx_ganador.UseVisualStyleBackColor = true;
            // 
            // tB_peticion
            // 
            tB_peticion.Location = new Point(1050, 314);
            tB_peticion.Name = "tB_peticion";
            tB_peticion.Size = new Size(139, 31);
            tB_peticion.TabIndex = 18;
            // 
            // lbl_contar
            // 
            lbl_contar.BorderStyle = BorderStyle.FixedSingle;
            lbl_contar.Font = new Font("Segoe UI", 12F, FontStyle.Regular, GraphicsUnit.Point);
            lbl_contar.Location = new Point(1137, 147);
            lbl_contar.Name = "lbl_contar";
            lbl_contar.Size = new Size(40, 34);
            lbl_contar.TabIndex = 19;
            // 
            // btn_enviar
            // 
            btn_enviar.Location = new Point(1039, 363);
            btn_enviar.Name = "btn_enviar";
            btn_enviar.Size = new Size(171, 48);
            btn_enviar.TabIndex = 20;
            btn_enviar.Text = "Realizar petición";
            btn_enviar.UseVisualStyleBackColor = true;
            btn_enviar.Click += btn_enviar_Click;
            // 
            // btn_invitar
            // 
            btn_invitar.Location = new Point(685, 481);
            btn_invitar.Name = "btn_invitar";
            btn_invitar.Size = new Size(129, 34);
            btn_invitar.TabIndex = 22;
            btn_invitar.Text = "Invitar";
            btn_invitar.UseVisualStyleBackColor = true;
            btn_invitar.Click += btn_invitar_Click;
            // 
            // lbl_iniciado
            // 
            lbl_iniciado.Anchor = AnchorStyles.Top | AnchorStyles.Right;
            lbl_iniciado.AutoSize = true;
            lbl_iniciado.Font = new Font("Perpetua Titling MT", 20F, FontStyle.Regular, GraphicsUnit.Point);
            lbl_iniciado.Location = new Point(1105, 22);
            lbl_iniciado.Name = "lbl_iniciado";
            lbl_iniciado.Size = new Size(143, 47);
            lbl_iniciado.TabIndex = 23;
            lbl_iniciado.Text = "label1";
            lbl_iniciado.TextAlign = ContentAlignment.MiddleRight;
            // 
            // btn_baja
            // 
            btn_baja.Font = new Font("Segoe UI", 7F, FontStyle.Regular, GraphicsUnit.Point);
            btn_baja.Location = new Point(1088, 75);
            btn_baja.Name = "btn_baja";
            btn_baja.Size = new Size(154, 31);
            btn_baja.TabIndex = 24;
            btn_baja.Text = "Darse de baja";
            btn_baja.UseVisualStyleBackColor = true;
            btn_baja.Click += btn_baja_Click;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(10F, 25F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1268, 553);
            Controls.Add(btn_baja);
            Controls.Add(lbl_iniciado);
            Controls.Add(btn_invitar);
            Controls.Add(btn_enviar);
            Controls.Add(lbl_contar);
            Controls.Add(tB_peticion);
            Controls.Add(cbx_ganador);
            Controls.Add(cbx_jugadas);
            Controls.Add(cbx_ganadas);
            Controls.Add(lbl_peticiones);
            Controls.Add(btn_salir);
            Controls.Add(lbl_conectados);
            Controls.Add(dgv_conectados);
            Controls.Add(lbl_Bienvenido);
            Controls.Add(btn_Olvidado);
            Controls.Add(btn_Iniciar);
            Controls.Add(btn_registro);
            Controls.Add(lbl_email);
            Controls.Add(lbl_contrasena);
            Controls.Add(lbl_nombre);
            Controls.Add(tB_email);
            Controls.Add(tB_contrasena);
            Controls.Add(tB_nombre);
            Name = "Form1";
            Text = "Form1";
            Load += Form1_Load;
            ((System.ComponentModel.ISupportInitialize)dgv_conectados).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion
        private TextBox tB_contrasena;
        private TextBox tB_email;
        private Label lbl_nombre;
        private Label lbl_contrasena;
        private Label lbl_email;
        private Button btn_registro;
        private Button btn_Iniciar;
        private Button btn_Olvidado;
        private Label lbl_Bienvenido;
        private DataGridView dgv_conectados;
        private Label lbl_conectados;
        private Button btn_salir;
        private Label lbl_peticiones;
        private CheckBox cbx_ganadas;
        private CheckBox cbx_jugadas;
        private CheckBox cbx_ganador;
        private TextBox tB_peticion;
        private Label lbl_contar;
        private Button btn_enviar;
        private Button btn_invitar;
        public TextBox tB_nombre;
        private Label lbl_iniciado;
        private Button btn_baja;
    }
}