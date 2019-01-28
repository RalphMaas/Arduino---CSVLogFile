namespace SM_SDCard
{
    using System;
    using System.Collections;
    using System.Collections.Generic;
    using System.ComponentModel;
    using Eco.Handles;
    using Eco.Services;
    using Eco.Persistence;
    using Eco.Wcf.Server;

    public partial class EcoProject1PMP : Eco.Persistence.PersistenceMapperProvider
    {
        public EcoProject1PMP() : base()
        {
            this.InitializeComponent();
        }

        /// <summary>
        /// Gets the singleton instance of the PersistenceMapperProvider.
        /// </summary>
        public static EcoProject1PMP Instance
        {
            get
            {
                return GetInstance<EcoProject1PMP>();
            }
        }

        /// <summary>
        /// Regenerates the database schema, no questions asked.
        /// </summary>
        public static void GenerateDB()
        {
            Instance.DoGenerateDB();
        }
        #region Eco Managed Code
        private void DoGenerateDB()
        {
            if (PersistenceMapper is PersistenceMapperDb)
            {
                (PersistenceMapper as PersistenceMapperDb).CreateDatabaseSchema(GetTypeSystemService(true), new DefaultCleanPsConfig(true));
            }
            else
            {
                throw new InvalidOperationException("The PersistenceMapper is not a PersistenceMapperDb");
            }
        }
        #endregion
    }

}
