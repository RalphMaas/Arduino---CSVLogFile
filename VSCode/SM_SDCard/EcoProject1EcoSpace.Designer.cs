namespace SM_SDCard
{
    public partial class EcoProject1EcoSpace
    {
        protected EcoProject1.EcoProject1Package IncludeEcoPackage_EcoProject1_EcoProject1Package;
        /// <summary>
        /// Required designer variable
        /// </summary>
        private System.ComponentModel.Container components = null;

        private Eco.Persistence.PersistenceMapperSharer persistenceMapperSharer1;
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                Active = false;
                if (this.components != null)
                {
                    this.components.Dispose();
                }
            }

            base.Dispose(disposing);
        }

        private void InitializeComponent()
        {

            this.persistenceMapperSharer1 = new Eco.Persistence.PersistenceMapperSharer();
            this.persistenceMapperSharer1.MapperProviderTypeName = "SM_SDCard.EcoProject1PMP";
            this.PersistenceMapper = this.persistenceMapperSharer1;
        }
    }
}
