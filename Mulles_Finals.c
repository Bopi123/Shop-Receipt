#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX 20

#define RESET    "\033[0m"
#define DIVIDER  "\033[38;5;230m"
#define YELLOW   "\033[38;5;221m"
#define TEXT     "\033[38;5;255m"
#define PROMPT   "\033[38;5;223m"
#define HEADER   "\033[38;5;229m"
#define ACCENT   "\033[38;5;180m"
#define ERROR    "\033[38;5;173m"

int main() {
    FILE *fp, *out;
    char items[MAX][20], item[20];
    int qty[MAX] = {0};
    float price[MAX] = {0};
    int count = 0, choice, i, j, found, inputQty;
    float inputPrice;

    printf(DIVIDER "===============================================================\n" RESET);
    printf(ACCENT  "            ALING BEBANG'S SCHOOL SUPPLIES STORE\n" RESET);
    printf(DIVIDER "===============================================================\n" RESET);
    printf(TEXT    "Press ENTER to continue..." RESET);
    getchar();

    fp = fopen("sales.txt", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%19s %d %f", item, &inputQty, &inputPrice) != EOF) {
            found = 0;
            for (i = 0; i < count; i++) {
                if (strcmp(items[i], item) == 0) {
                    qty[i] += inputQty;
                    found = 1;
                    break;
                }
            }
            if (!found && count < MAX) {
                strcpy(items[count], item);
                qty[count] = inputQty;
                price[count] = inputPrice;
                count++;
            }
        }
        fclose(fp);
    }

    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (strcmp(items[i], items[j]) > 0) {
                char tItem[20];
                int tQty;
                float tPrice;

                strcpy(tItem, items[i]);
                strcpy(items[i], items[j]);
                strcpy(items[j], tItem);

                tQty = qty[i];
                qty[i] = qty[j];
                qty[j] = tQty;

                tPrice = price[i];
                price[i] = price[j];
                price[j] = tPrice;
            }
        }
    }

    do {
        printf(DIVIDER "\n===== MENU =====\n" RESET);
        printf(YELLOW "1." RESET " " TEXT "Show Receipt\n" RESET);
        printf(YELLOW "2." RESET " " TEXT "Add Item\n" RESET);
        printf(YELLOW "3." RESET " " TEXT "Search Best Selling Item\n" RESET);
        printf(YELLOW "4." RESET " " TEXT "Remove Item\n" RESET);
        printf(YELLOW "5." RESET " " TEXT "Save Receipt\n" RESET);
        printf(YELLOW "6." RESET " " TEXT "Exit\n" RESET);
        printf(PROMPT "Enter choice: " RESET);
        scanf("%d", &choice);

        switch (choice) {

        case 1: {
            float grandTotal = 0;
            int best = 0;

            if (count == 0) {
                printf(ERROR "No items available.\n" RESET);
                break;
            }

            printf(DIVIDER "\n===============================================================\n" RESET);
            printf(ACCENT "                     ALING BEBANG'S STORE\n" RESET);
            printf(DIVIDER "===============================================================\n" RESET);

            printf(HEADER "| %-3s | %-15s | %-10s | %-10s | %-10s |\n" RESET,
                   "No", "Item", "Price", "Quantity", "Total");
            printf(DIVIDER "---------------------------------------------------------------\n" RESET);

            for (i = 0; i < count; i++) {
                float total = price[i] * qty[i];
                printf(TEXT "| %-3d | %-15s | %-10.2f | %-10d | %-10.2f |\n" RESET,
                       i + 1, items[i], price[i], qty[i], total);

                grandTotal += total;
                if (qty[i] > qty[best]) best = i;
            }

            printf(DIVIDER "---------------------------------------------------------------\n" RESET);
            printf(PROMPT "GRAND TOTAL: %.2f\n" RESET, grandTotal);
            break;
        }

        case 2:
            printf(PROMPT "\nEnter item name: " RESET);
            getchar();
            fgets(item, 20, stdin);
            item[strcspn(item, "\n")] = 0;

            printf(PROMPT "Enter quantity: " RESET);
            scanf("%d", &inputQty);

            printf(PROMPT "Enter price: " RESET);
            scanf("%f", &inputPrice);

            found = 0;
            for (i = 0; i < count; i++) {
                if (strcmp(items[i], item) == 0) {
                    qty[i] += inputQty;
                    price[i] = inputPrice;
                    found = 1;
                    break;
                }
            }

            if (!found && count < MAX) {
                strcpy(items[count], item);
                qty[count] = inputQty;
                price[count] = inputPrice;
                count++;
            }

            for (i = 0; i < count - 1; i++) {
                for (j = i + 1; j < count; j++) {
                    if (strcmp(items[i], items[j]) > 0) {
                        char tItem[20];
                        int tQty;
                        float tPrice;

                        strcpy(tItem, items[i]);
                        strcpy(items[i], items[j]);
                        strcpy(items[j], tItem);

                        tQty = qty[i];
                        qty[i] = qty[j];
                        qty[j] = tQty;

                        tPrice = price[i];
                        price[i] = price[j];
                        price[j] = tPrice;
                    }
                }
            }

            fp = fopen("sales.txt", "w");
            if (fp != NULL) {
                for (i = 0; i < count; i++)
                    fprintf(fp, "%s %d %.2f\n", items[i], qty[i], price[i]);
                fclose(fp);
            }

            printf(PROMPT "Item added!\n" RESET);
            break;

        case 3: {
            int best = 0;

            if (count == 0) {
                printf(ERROR "No data available.\n" RESET);
                break;
            }

            for (i = 1; i < count; i++)
                if (qty[i] > qty[best]) best = i;

            printf(DIVIDER "\n=========== BEST SELLING ITEM ===========\n" RESET);
            printf(TEXT "Item Name : %s\n" RESET, items[best]);
            printf(TEXT "Quantity  : %d\n" RESET, qty[best]);
            printf(TEXT "Price     : %.2f\n" RESET, price[best]);
            printf(TEXT "Total     : %.2f\n" RESET, price[best] * qty[best]);
            printf(DIVIDER "========================================\n" RESET);
            break;
        }

        case 4:
            printf(PROMPT "\nEnter item name to remove: " RESET);
            getchar();
            fgets(item, 20, stdin);
            item[strcspn(item, "\n")] = 0;

            found = 0;
            for (i = 0; i < count; i++) {
                if (strcmp(items[i], item) == 0) {
                    for (j = i; j < count - 1; j++) {
                        strcpy(items[j], items[j + 1]);
                        qty[j] = qty[j + 1];
                        price[j] = price[j + 1];
                    }
                    count--;
                    found = 1;
                    printf(PROMPT "Item removed.\n" RESET);
                    break;
                }
            }

            if (!found)
                printf(ERROR "Item not found.\n" RESET);

            fp = fopen("sales.txt", "w");
            if (fp != NULL) {
                for (i = 0; i < count; i++)
                    fprintf(fp, "%s %d %.2f\n", items[i], qty[i], price[i]);
                fclose(fp);
            }
            break;

        case 5: {
            float grandTotal = 0;
            int best = 0;
            time_t now;
            struct tm *t;

            out = fopen("sales_summary.txt", "w");
            if (out == NULL) {
                printf(ERROR "Cannot create file.\n" RESET);
                break;
            }

            time(&now);
            t = localtime(&now);

            fprintf(out, "DATE & TIME: %04d-%02d-%02d %02d:%02d:%02d\n",
                    t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                    t->tm_hour, t->tm_min, t->tm_sec);

            fprintf(out, "===============================================================\n");
            fprintf(out, "                     ALING BEBANG'S STORE\n");
            fprintf(out, "===============================================================\n");

            fprintf(out, "| %-3s | %-15s | %-10s | %-10s | %-10s |\n",
                    "No", "Item", "Price", "Quantity", "Total");
            fprintf(out, "---------------------------------------------------------------\n");

            for (i = 0; i < count; i++) {
                float total = price[i] * qty[i];
                fprintf(out, "| %-3d | %-15s | %-10.2f | %-10d | %-10.2f |\n",
                        i + 1, items[i], price[i], qty[i], total);

                grandTotal += total;
                if (qty[i] > qty[best]) best = i;
            }

            fprintf(out, "---------------------------------------------------------------\n");
            fprintf(out, "GRAND TOTAL: %.2f\n", grandTotal);
            fprintf(out, "BEST SELLER: %s (Quantity: %d)\n",
                    items[best], qty[best]);

            fclose(out);
            printf(PROMPT "Receipt saved!\n" RESET);
            break;
        }

        case 6:
            printf(TEXT "Thank You!\n" RESET);
            break;

        default:
            printf(ERROR "Invalid choice.\n" RESET);
        }

    } while (choice != 6);

    return 0;
}
