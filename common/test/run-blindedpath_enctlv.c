#include "../bigsize.c"
#include "../blinding.c"
#include "../hmac.c"
#include "../type_to_string.c"
#include <common/setup.h>
#include <stdio.h>

#undef SUPERVERBOSE
#define SUPERVERBOSE printf
  #include "../blindedpath.c"

/* AUTOGENERATED MOCKS START */
/* Generated stub for amount_asset_is_main */
bool amount_asset_is_main(struct amount_asset *asset UNNEEDED)
{ fprintf(stderr, "amount_asset_is_main called!\n"); abort(); }
/* Generated stub for amount_asset_to_sat */
struct amount_sat amount_asset_to_sat(struct amount_asset *asset UNNEEDED)
{ fprintf(stderr, "amount_asset_to_sat called!\n"); abort(); }
/* Generated stub for amount_sat */
struct amount_sat amount_sat(u64 satoshis UNNEEDED)
{ fprintf(stderr, "amount_sat called!\n"); abort(); }
/* Generated stub for amount_sat_add */
 bool amount_sat_add(struct amount_sat *val UNNEEDED,
				       struct amount_sat a UNNEEDED,
				       struct amount_sat b UNNEEDED)
{ fprintf(stderr, "amount_sat_add called!\n"); abort(); }
/* Generated stub for amount_sat_eq */
bool amount_sat_eq(struct amount_sat a UNNEEDED, struct amount_sat b UNNEEDED)
{ fprintf(stderr, "amount_sat_eq called!\n"); abort(); }
/* Generated stub for amount_sat_greater_eq */
bool amount_sat_greater_eq(struct amount_sat a UNNEEDED, struct amount_sat b UNNEEDED)
{ fprintf(stderr, "amount_sat_greater_eq called!\n"); abort(); }
/* Generated stub for amount_sat_sub */
 bool amount_sat_sub(struct amount_sat *val UNNEEDED,
				       struct amount_sat a UNNEEDED,
				       struct amount_sat b UNNEEDED)
{ fprintf(stderr, "amount_sat_sub called!\n"); abort(); }
/* Generated stub for amount_sat_to_asset */
struct amount_asset amount_sat_to_asset(struct amount_sat *sat UNNEEDED, const u8 *asset UNNEEDED)
{ fprintf(stderr, "amount_sat_to_asset called!\n"); abort(); }
/* Generated stub for amount_tx_fee */
struct amount_sat amount_tx_fee(u32 fee_per_kw UNNEEDED, size_t weight UNNEEDED)
{ fprintf(stderr, "amount_tx_fee called!\n"); abort(); }
/* Generated stub for fromwire_amount_msat */
struct amount_msat fromwire_amount_msat(const u8 **cursor UNNEEDED, size_t *max UNNEEDED)
{ fprintf(stderr, "fromwire_amount_msat called!\n"); abort(); }
/* Generated stub for fromwire_amount_sat */
struct amount_sat fromwire_amount_sat(const u8 **cursor UNNEEDED, size_t *max UNNEEDED)
{ fprintf(stderr, "fromwire_amount_sat called!\n"); abort(); }
/* Generated stub for towire_amount_msat */
void towire_amount_msat(u8 **pptr UNNEEDED, const struct amount_msat msat UNNEEDED)
{ fprintf(stderr, "towire_amount_msat called!\n"); abort(); }
/* Generated stub for towire_amount_sat */
void towire_amount_sat(u8 **pptr UNNEEDED, const struct amount_sat sat UNNEEDED)
{ fprintf(stderr, "towire_amount_sat called!\n"); abort(); }
/* AUTOGENERATED MOCKS END */

static void json_strfield(const char *name, const char *val)
{
	printf("\t\"%s\": \"%s\",\n", name, val);
}

int main(int argc, char *argv[])
{
	struct privkey alice, bob, carol, dave, blinding, override_blinding;
	struct pubkey alice_id, bob_id, carol_id, dave_id, blinding_pub, override_blinding_pub, alias;
	u8 *enctlv;

	common_setup(argv[0]);

	memset(&alice, 'A', sizeof(alice));
	memset(&bob, 'B', sizeof(bob));
	memset(&carol, 'C', sizeof(carol));
	memset(&dave, 'D', sizeof(dave));
	pubkey_from_privkey(&alice, &alice_id);
	pubkey_from_privkey(&bob, &bob_id);
	pubkey_from_privkey(&carol, &carol_id);
	pubkey_from_privkey(&dave, &dave_id);

	memset(&blinding, 5, sizeof(blinding));
	pubkey_from_privkey(&blinding, &blinding_pub);

	/* We output the JSON test vectors. */
	printf("[{");
	json_strfield("test name", "Simple enctlv for Alice, next is Bob");
	json_strfield("node_privkey",
		      type_to_string(tmpctx, struct privkey, &alice));
	json_strfield("node_id",
		      type_to_string(tmpctx, struct pubkey, &alice_id));
	json_strfield("blinding_secret",
		      type_to_string(tmpctx, struct privkey, &blinding));
	json_strfield("blinding",
		      type_to_string(tmpctx, struct pubkey, &blinding_pub));
	printf("\t\"encmsg\": {\n"
	       "\t\t\"next_node_id\": \"%s\"\n"
	       "\t},\n",
	       type_to_string(tmpctx, struct pubkey, &bob_id));

	enctlv = create_enctlv(tmpctx, &blinding, &alice_id, &bob_id,
			       0, NULL, &blinding, &alias);
	printf("\t\"enctlv_hex\": \"%s\"\n"
	       "},\n",
	       tal_hex(tmpctx, enctlv));

	pubkey_from_privkey(&blinding, &blinding_pub);
	memset(&override_blinding, 7, sizeof(override_blinding));
	pubkey_from_privkey(&override_blinding, &override_blinding_pub);

	printf("{");
	json_strfield("test name",
		      "Blinding-key-override enctlv for Bob, next is Carol");
	json_strfield("node_privkey",
		      type_to_string(tmpctx, struct privkey, &bob));
	json_strfield("node_id",
		      type_to_string(tmpctx, struct pubkey, &bob_id));
	json_strfield("blinding_secret",
		      type_to_string(tmpctx, struct privkey, &blinding));
	json_strfield("blinding",
		      type_to_string(tmpctx, struct pubkey, &blinding_pub));
	printf("\t\"encmsg\": {\n"
	       "\t\t\"next_node_id\": \"%s\",\n"
	       "\t\t\"blinding\": \"%s\"\n"
	       "\t},\n",
	       type_to_string(tmpctx, struct pubkey, &carol_id),
	       type_to_string(tmpctx, struct privkey, &override_blinding));

	enctlv = create_enctlv(tmpctx, &blinding, &bob_id, &carol_id,
			       0, &override_blinding_pub, &blinding, &alias);
	printf("\t\"enctlv_hex\": \"%s\"\n"
	       "},\n",
	       tal_hex(tmpctx, enctlv));

	/* That replaced the blinding */
	blinding = override_blinding;
	blinding_pub = override_blinding_pub;

	printf("{");
	json_strfield("test name", "Padded enctlv for Carol, next is Dave");
	json_strfield("node_privkey",
		      type_to_string(tmpctx, struct privkey, &carol));
	json_strfield("node_id",
		      type_to_string(tmpctx, struct pubkey, &carol_id));
	json_strfield("blinding_secret",
		      type_to_string(tmpctx, struct privkey, &blinding));
	json_strfield("blinding",
		      type_to_string(tmpctx, struct pubkey, &blinding_pub));
	printf("\t\"encmsg\": {\n"
	       "\t\t\"next_node_id\": \"%s\",\n"
	       "\t\t\"padding\": \"%s\"\n"
	       "\t},\n",
	       type_to_string(tmpctx, struct pubkey, &dave_id),
	       tal_hex(tmpctx, tal_arrz(tmpctx, u8, 35)));

	enctlv = create_enctlv(tmpctx, &blinding, &carol_id, &dave_id,
			       35, NULL, &blinding, &alias);
	printf("\t\"enctlv_hex\": \"%s\"\n"
	       "}]\n",
	       tal_hex(tmpctx, enctlv));

	common_shutdown();
}