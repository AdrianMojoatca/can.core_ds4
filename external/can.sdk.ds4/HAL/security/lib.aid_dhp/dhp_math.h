
#define DH_VAR_LSW 0
#define DH_VAR_MSW 3
typedef UInt32 DH_VAR[4]; // little-endian, aligned 32 bit for performance


extern DH_VAR dh_acc;


void dh_load_nbo(const UInt8 *from);
void dh_store_nbo(UInt8 *to);


void mov128(DH_VAR to, DH_VAR from);
void dh_set(UInt8 quick);
unsigned add128(DH_VAR a, DH_VAR b, DH_VAR sum);
void dh_setP(void);
void dh_add(DH_VAR what);
void dh_mul(DH_VAR what);
void dh_exp(DH_VAR exp);
